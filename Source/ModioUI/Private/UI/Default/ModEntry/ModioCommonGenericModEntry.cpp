/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/ModEntry/ModioCommonGenericModEntry.h"

#include "ModioUI.h"
#include "TimerManager.h"
#include "Components/ListView.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "Core/ModioModInfoUI.h"
#include "Libraries/ModioSDKLibrary.h"
#include "UI/Default/Dialog/ModioCommonDialogInfo.h"
#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"
#include "UI/Foundation/Components/Image/ModioCommonDynamicImage.h"
#include "UI/Foundation/Components/Image/ModioCommonDynamicImageStyle.h"
#include "UI/Default/ModEntry/ModioCommonGenericModEntryStyle.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Base/Image/ModioCommonImageBase.h"
#include "UI/Foundation/Base/ModBrowser/ModioCommonModBrowserBase.h"
#include "UI/Foundation/Components/Border/ModioCommonBorder.h"
#include "UI/Foundation/Components/Border/ModioCommonBorderStyle.h"
#include "UI/Foundation/Components/ProgressBar/ModioCommonProgressBar.h"
#include "UI/Foundation/Components/ProgressBar/ModioCommonProgressBarStyle.h"
#include "UI/Foundation/Components/Text/RichTextBlock/ModioCommonRichTextBlock.h"
#include "UI/Foundation/Components/Text/RichTextBlock/ModioCommonRichTextStyle.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerUserWidget.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerUserWidgetStyle.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerWidget.h"
#include "UI/Interfaces/IModioModBrowser.h"
#include "UI/Settings/ModioCommonUISettings.h"
#include "UI/Settings/Params/ModioCommonModEntryParams.h"

void UModioCommonGenericModEntry::SetStyle(TSubclassOf<UModioCommonGenericModEntryStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

bool UModioCommonGenericModEntry::Initialize()
{
	const bool bSuperInitialized = Super::Initialize();
	if (ModOperationTrackerWidget)
	{
		ModOperationTrackerWidget->OnProgressFast.RemoveAll(this);
		ModOperationTrackerWidget->OnProgressFast.AddWeakLambda(this, [this](FModioUnsigned64 Current, FModioUnsigned64 Total) {
			UpdateOperationProgressPercent(Total ? (Current / static_cast<double>(Total)) : 0);
			UpdateOperationProgressBytes(Current, Total);
		});

		ModOperationTrackerWidget->OnSpeedFast.RemoveAll(this);
		ModOperationTrackerWidget->OnSpeedFast.AddWeakLambda(this, [this](FModioUnsigned64 DeltaBytes, double DeltaTime) {
			if (DeltaBytes.Underlying > 0)
			{
				UpdateSpeed(DeltaBytes, DeltaTime);
			}
		});
		UpdateOperationProgressPercent(0);
		UpdateOperationProgressBytes(FModioUnsigned64(0), FModioUnsigned64(0));
		UpdateSpeed(FModioUnsigned64(0), 0);
	}
	return bSuperInitialized;
}

void UModioCommonGenericModEntry::NativePreConstruct()
{
	Super::NativePreConstruct();
#if WITH_EDITOR
	if (IsDesignTime())
	{
		UModioModInfoUI* ModInfo = NewObject<UModioModInfoUI>();
		if (ModInfo)
		{
			ModInfo->Underlying = PreviewModInfo;
			SetDataSource(ModInfo);
		}
	}
#endif
}

void UModioCommonGenericModEntry::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (UWorld* World = GetWorld()) 
	{
		World->GetTimerManager().ClearTimer(DeselectTimerHandle);
	}
	if (IsModListItemValid())
	{
		SetFocus();
		SelectModListItem();
	}
}

void UModioCommonGenericModEntry::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (IsModListItemValid())
	{
		if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>()) 
		{
			if (UWorld* World = GetWorld()) 
			{
				World->GetTimerManager().ClearTimer(DeselectTimerHandle);
				World->GetTimerManager().SetTimer(DeselectTimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]() 
				{ 
					DeselectModListItem();								
				}), UISettings->ModEntryParams.DeselectionDelay, false);
			}
		}
	}
}

void UModioCommonGenericModEntry::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	ClearListeningInputActions();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>()) 
	{
		if (SpeedLabelTextBlock)
		{
			SpeedLabelTextBlock->SetText(UISettings->ModEntryParams.SpeedLabel);
			SwitchSpeedVisibility(false);
		}

		if (SizeLabelTextBlock)
		{
			SizeLabelTextBlock->SetText(UISettings->ModEntryParams.SizeLabel);
		}

		if (CancelButton)
		{
			CancelButton->SetLabel(UISettings->ModEntryParams.CancelLabel);
		}

		if (EnableButton)
		{
			EnableButton->SetLabel(UISettings->ModEntryParams.EnableLabel);
		}

		if (DisableButton)
		{
			DisableButton->SetLabel(UISettings->ModEntryParams.DisableLabel);
		}

		if (OpenModDetailsButton)
		{
			OpenModDetailsButton->SetLabel(UISettings->ModEntryParams.OpenModDetailsLabel);
		}

		if (PrioritizeTransferButton)
		{
			PrioritizeTransferButton->SetLabel(UISettings->ModEntryParams.PrioritizeTransferLabel);
		}
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to synchronize properties for mod entry '%s': Settings are invalid"), *GetName());
	}

	if (!DataSource)
	{
		return;
	}

	const FModioModInfo ModInfo = Execute_GetFullModInfo(this);

	const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>();
	if (!UISettings)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to set data source for mod entry '%s': Settings are invalid"), *GetName());
		return;
	}

	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to set data source for mod entry '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	bObtainedModImage = false;
	Subsystem->RequestLogoDownloadForModID(ModInfo.ModId);

	// Retrieving beforehand to avoid multiple calls to check mod states
	const bool bModInstalled = Execute_IsModInstalled(this);
	const bool bModDownloading = Execute_IsModDownloading(this);
	const bool bModExtracting = Execute_IsModExtracting(this);
	const bool bModInQueue = Execute_IsModInQueue(this);
	const bool bModEnabled = Execute_IsModEnabled(this);

	if (
#if WITH_EDITOR
		!IsDesignTime() &&
#endif
		ModNameTextBlock)
	{
		ModNameTextBlock->SetText(FText::FromString(ModInfo.ProfileName));
	}
	
	if (OperationLabelTextBlock)
	{
		OperationLabelTextBlock->SetText(bModDownloading ? UISettings->ModEntryParams.DownloadingLabel : bModExtracting ? UISettings->ModEntryParams.ExtractingLabel : bModInQueue ? UISettings->ModEntryParams.QueuedLabel : FText::GetEmpty());
	}

	// Downloads
	{
		if (DownloadsDetailsTextBlock)
		{
			DownloadsDetailsTextBlock->SetText(FText::FromString(UModioSDKLibrary::GetShortenedNumberAsString(ModInfo.Stats.DownloadsTotal)));
		} 
	}

	// Size
	{
		if (SizeDetailsTextBlock)
		{
			const FText FileSizeText = UModioSDKLibrary::Filesize_ToString(ModInfo.FileInfo.Filesize, MinDecimalsFileSize, MaxDecimalsFileSize);
			SizeDetailsTextBlock->SetText(FileSizeText);
		}
	}

	// Rating
	{
		if (RatingPercentageTextBlock)
		{
			RatingPercentageTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%lld"), ModInfo.Stats.RatingPercentagePositive) + " %"));
		}

		if (RatingPositiveTextBlock)
		{
			RatingPositiveTextBlock->SetText(FText::FromString(FString::FormatAsNumber(ModInfo.Stats.RatingPositive)));
		}

		if (RatingNegativeTextBlock)
		{
			RatingNegativeTextBlock->SetText(FText::FromString(FString::FormatAsNumber(ModInfo.Stats.RatingNegative)));
		}
	}
	
	if (SubscribeButton)
	{
		SubscribeButton->SetLabel(Execute_IsModSubscribed(this) ? UISettings->ModEntryParams.UnsubscribeLabel : UISettings->ModEntryParams.SubscribeLabel);
	}

	if (ForceUninstallButton)
	{
		ForceUninstallButton->SetLabel(UISettings->ModEntryParams.ForceUninstallLabel);
	}
	
	if (bModInstalled && Subsystem->GetIsCollectionModDisableUIEnabled())
	{
		SwitchEnableButtonVisibility(!bModEnabled);
		SwitchDisableButtonVisibility(bModEnabled);

		UModioCommonButtonBase* EnableDisableButton = bModEnabled ? DisableButton : EnableButton;
		if (EnableDisableButton)
		{
			ListenForInputAction(EnableDisableButton, UISettings->ModEntryParams.SwitchEnabledInputAction, bModEnabled ? UISettings->ModEntryParams.DisableLabel : UISettings->ModEntryParams.EnableLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				HandleSwitchEnabledClicked();
			}));
		}
	}
	else
	{
		SwitchEnableButtonVisibility(false);
		SwitchDisableButtonVisibility(false);
	}

	if (SubscribeButton)
	{
		ListenForInputAction(SubscribeButton, UISettings->ModEntryParams.SubscribeInputAction, Execute_IsModSubscribed(this) ? UISettings->ModEntryParams.UnsubscribeLabel : UISettings->ModEntryParams.SubscribeLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
			HandleSubscribeClicked();
		}));
	}

	if (bModDownloading || bModExtracting || bModInQueue)
	{
		SwitchCancelButtonVisibility(true);
		if (CancelButton)
		{
			ListenForInputAction(CancelButton, UISettings->ModEntryParams.CancelInputAction, UISettings->ModEntryParams.CancelLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				HandleCancelClicked();
			}));
		}
	}
	else
	{
		SwitchCancelButtonVisibility(false);
	}
	
	if (OpenModDetailsButton)
	{
		ListenForInputAction(OpenModDetailsButton, UISettings->ModEntryParams.OpenModDetailsInputAction, UISettings->ModEntryParams.OpenModDetailsLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
			HandleOpenModDetailsClicked();
		}));
	}

	if (bModInQueue)
	{
		if (PrioritizeTransferButton)
		{
			ListenForInputAction(PrioritizeTransferButton, UISettings->ModEntryParams.PrioritizeTransferInputAction, UISettings->ModEntryParams.PrioritizeTransferLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				HandlePrioritizeTransferClicked();
			}));
		}
	}

	UModioModCollectionEntryUI* CollectionEntry = Cast<UModioModCollectionEntryUI>(DataSource);
	if (CollectionEntry && CollectionEntry->bCachedSubscriptionStatus == false)
	{
		SwitchForceUninstallButtonVisibility(true);

		if (ForceUninstallButton)
		{
			ListenForInputAction(ForceUninstallButton, UISettings->ModEntryParams.ForceUninstallInputAction, UISettings->ModEntryParams.ForceUninstallLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				HandleForceUninstallClicked();
			}));
		}
	}
	else
	{
		SwitchForceUninstallButtonVisibility(false);
	}

	UpdateInputActions();
}

void UModioCommonGenericModEntry::NativeUpdateStyling(bool bIsListItemSelected)
{
	if (!ModioStyle)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to update styling for mod entry '%s': ModioStyle is invalid"), *GetName());
		Super::NativeUpdateStyling(bIsListItemSelected);
		return;
	}

	TSubclassOf<UModioCommonGenericModEntryStateStyle> CurrentStateStyle = bIsListItemSelected
		                                                                     ? ModioStyle.GetDefaultObject()->SelectedStyle
		                                                                     : ModioStyle.GetDefaultObject()->NormalStyle;

	if (!CurrentStateStyle)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to update styling for mod entry '%s': CurrentStateStyle is invalid"), *GetName());
		Super::NativeUpdateStyling(bIsListItemSelected);
		return;
	}

	if (ModOperationTrackerUserWidget)
	{
		if (TSubclassOf<UModioCommonModOperationTrackerUserWidgetStyle> ModOperationTrackerUserWidgetStyle = CurrentStateStyle.GetDefaultObject()->ModOperationTrackerStyle)
		{
			ModOperationTrackerUserWidget->SetStyle(ModOperationTrackerUserWidgetStyle);
		}
	}

	if (Background)
	{
		if (TSubclassOf<UModioCommonBorderStyle> BackgroundStyle = CurrentStateStyle.GetDefaultObject()->BackgroundStyle)
		{
			Background->SetStyle(BackgroundStyle);
		}
	}
	
	if (ModImage)
	{
		if (TSubclassOf<UModioCommonDynamicImageStyle> ImageStyle = CurrentStateStyle.GetDefaultObject()->ModImageStyle)
		{
			ModImage->SetStyle(ImageStyle);
		}
	}

	if (ModNameTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> ModNameTextStyle = CurrentStateStyle.GetDefaultObject()->ModNameTextStyle)
		{
			ModNameTextBlock->SetStyle(ModNameTextStyle);
		}
	}

	if (OperationLabelTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> OperationLabelTextStyle = CurrentStateStyle.GetDefaultObject()->OperationLabelTextStyle)
		{
			OperationLabelTextBlock->SetStyle(OperationLabelTextStyle);
		}
	}

	if (OperationDetailsTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> OperationDetailsTextStyle = CurrentStateStyle.GetDefaultObject()->OperationDetailsTextStyle)
		{
			OperationDetailsTextBlock->SetStyle(OperationDetailsTextStyle);
		}
	}

	if (SpeedLabelTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> SpeedLabelTextStyle = CurrentStateStyle.GetDefaultObject()->SpeedLabelTextStyle)
		{
			SpeedLabelTextBlock->SetStyle(SpeedLabelTextStyle);
		}
	}

	if (SpeedDetailsTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> SpeedDetailsTextStyle = CurrentStateStyle.GetDefaultObject()->SpeedDetailsTextStyle)
		{
			SpeedDetailsTextBlock->SetStyle(SpeedDetailsTextStyle);
		}
	}

	if (DownloadsLabelTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> DownloadsLabelTextStyle = CurrentStateStyle.GetDefaultObject()->DownloadsLabelTextStyle)
		{
			DownloadsLabelTextBlock->SetStyle(DownloadsLabelTextStyle);
		}
	}

	if (DownloadsDetailsTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> DownloadsDetailsTextStyle = CurrentStateStyle.GetDefaultObject()->DownloadsDetailsTextStyle)
		{
			DownloadsDetailsTextBlock->SetStyle(DownloadsDetailsTextStyle);
		}
	}

	if (SizeLabelTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> SizeLabelTextStyle = CurrentStateStyle.GetDefaultObject()->SizeLabelTextStyle)
		{
			SizeLabelTextBlock->SetStyle(SizeLabelTextStyle);
		}
	}

	if (SizeDetailsTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> SizeDetailsTextStyle = CurrentStateStyle.GetDefaultObject()->SizeDetailsTextStyle)
		{
			SizeDetailsTextBlock->SetStyle(SizeDetailsTextStyle);
		}
	}

	if (RatingPercentageTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> RatingPercentageTextStyle = CurrentStateStyle.GetDefaultObject()->RatingPercentageTextStyle)
		{
			RatingPercentageTextBlock->SetStyle(RatingPercentageTextStyle);
		}
	}

	if (RatingPositiveTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> RatingPositiveTextStyle = CurrentStateStyle.GetDefaultObject()->RatingPositiveTextStyle)
		{
			RatingPositiveTextBlock->SetStyle(RatingPositiveTextStyle);
		}
	}

	if (RatingNegativeTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> RatingNegativeTextStyle = CurrentStateStyle.GetDefaultObject()->RatingNegativeTextStyle)
		{
			RatingNegativeTextBlock->SetStyle(RatingNegativeTextStyle);
		}
	}

	if (SubscribeButton)
	{
		if (TSubclassOf<UModioCommonButtonStyle> SubscribeButtonStyle = CurrentStateStyle.GetDefaultObject()->SubscribeButtonStyle)
		{
			SubscribeButton->SetStyle(SubscribeButtonStyle);
		}
	}

	if (CancelButton)
	{
		if (TSubclassOf<UModioCommonButtonStyle> CancelButtonStyle = CurrentStateStyle.GetDefaultObject()->CancelButtonStyle)
		{
			CancelButton->SetStyle(CancelButtonStyle);
		}
	}

	if (EnableButton)
	{
		if (TSubclassOf<UModioCommonButtonStyle> EnableButtonStyle = CurrentStateStyle.GetDefaultObject()->EnableButtonStyle)
		{
			EnableButton->SetStyle(EnableButtonStyle);
		}
	}

	if (DisableButton)
	{
		if (TSubclassOf<UModioCommonButtonStyle> DisableButtonStyle = CurrentStateStyle.GetDefaultObject()->DisableButtonStyle)
		{
			DisableButton->SetStyle(DisableButtonStyle);
		}
	}

	if (OpenModDetailsButton)
	{
		if (TSubclassOf<UModioCommonButtonStyle> OpenModDetailsButtonStyle = CurrentStateStyle.GetDefaultObject()->OpenModDetailsButtonStyle)
		{
			OpenModDetailsButton->SetStyle(OpenModDetailsButtonStyle);
		}
	}

	if (PercentProgressTextBlock)
	{
		if (TSubclassOf<UModioCommonRichTextStyle> PercentProgressStyle = CurrentStateStyle.GetDefaultObject()->PercentProgressTextStyle)
		{
			PercentProgressTextBlock->SetStyle(PercentProgressStyle);
		}
	}

	if (PercentProgressBar)
	{
		if (TSubclassOf<UModioCommonProgressBarStyle> PercentProgressBarStyle = CurrentStateStyle.GetDefaultObject()->PercentProgressBarStyle)
		{
			PercentProgressBar->SetStyle(PercentProgressBarStyle);
		}
	}

	Super::NativeUpdateStyling(bIsListItemSelected);
}

void UModioCommonGenericModEntry::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	const FModioModInfo CurrentModInfo = Execute_GetFullModInfo(this);
#if WITH_EDITOR
	if (!IsDesignTime())
#endif
	{
		UpdateOperationProgressBytes(FModioUnsigned64(0), FModioUnsigned64(CurrentModInfo.FileInfo.Filesize));
		UpdateOperationProgressPercent(0);
	}

	if (Execute_IsModDownloading(this) || Execute_IsModExtracting(this))
	{
		UpdateSpeed(FModioUnsigned64(), 0);
	}

	SwitchSpeedVisibility(false);
}

void UModioCommonGenericModEntry::NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize)
{
	Super::NativeOnModLogoDownloadCompleted(ModID, ErrorCode, Image, LogoSize);

	if (!bObtainedModImage && Execute_GetModID(this) == ModID)
	{
		bObtainedModImage = true;
		if (ErrorCode)
		{
			UE_LOG(ModioUI, Error, TEXT("Unable to download mod logo for mod entry '%s': '%s' - %d"), *GetName(), *ErrorCode.GetErrorMessage(), ErrorCode.GetValue());
			if (ModImage)
			{
				ModImage->OnLoadNewImageError();
			}
		}
		else if (ModImage)
		{
			ModImage->LoadImageFromFileAsync(Image.GetValue());
		}
	}
}

void UModioCommonGenericModEntry::SwitchForceUninstallButtonVisibility_Implementation(bool bIsVisible)
{
	if (ForceUninstallButton)
	{
		ForceUninstallButton->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonGenericModEntry::SwitchCancelButtonVisibility_Implementation(bool bIsVisible)
{
	if (CancelButton)
	{
		CancelButton->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonGenericModEntry::SwitchEnableButtonVisibility_Implementation(bool bIsVisible)
{
	if (EnableButton)
	{
		EnableButton->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonGenericModEntry::SwitchDisableButtonVisibility_Implementation(bool bIsVisible)
{
	if (DisableButton)
	{
		DisableButton->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonGenericModEntry::OnRatingSubmissionComplete_Implementation(FModioErrorCode ErrorCode, EModioRating Rating)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->DisplayNotificationParams(UModioNotificationParamsLibrary::CreateRatingNotification(ErrorCode, DataSource.Get()));
	}
	if (!ErrorCode)
	{
		// TODO: Update the rating
		SetDataSource(DataSource);
	}
}

void UModioCommonGenericModEntry::UpdateOperationProgressPercent_Implementation(float InPercent)
{
	if (PercentProgressBar)
	{
		PercentProgressBar->SetPercent(InPercent);
	}

	if (PercentProgressTextBlock)
	{
		PercentProgressTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), FMath::RoundToInt(InPercent * 100.f))));
	}

	SynchronizeProperties();
}

void UModioCommonGenericModEntry::UpdateOperationProgressBytes_Implementation(FModioUnsigned64 Current, FModioUnsigned64 Total)
{
	const EFileSizeUnit FileSizeUnit = UModioSDKLibrary::GetDesiredFileSizeUnit(Total.Underlying);
	const FText CurrentText = UModioSDKLibrary::Filesize_ToString(Current.Underlying, MinDecimalsProgress, MaxDecimalsProgress, FileSizeUnit, false);
	const FText TotalText = UModioSDKLibrary::Filesize_ToString(Total.Underlying, MinDecimalsProgress, MaxDecimalsProgress, FileSizeUnit);
	if (OperationDetailsTextBlock)
	{
		OperationDetailsTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s / %s"), *CurrentText.ToString(), *TotalText.ToString())));
	}
}

void UModioCommonGenericModEntry::UpdateSpeed_Implementation(FModioUnsigned64 DeltaBytes, double DeltaTime)
{
	SwitchSpeedVisibility(true);
	if (SpeedDetailsTextBlock)
	{
		const int64 BytesPerSecond = DeltaBytes.Underlying <= 0 ? 0 : DeltaBytes / DeltaTime / 8;
		const FText TotalText = UModioSDKLibrary::Filesize_ToString(BytesPerSecond, MinDecimalsSpeed, MaxDecimalsSpeed);
		SpeedDetailsTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s/s"), *TotalText.ToString())));
	}
}

void UModioCommonGenericModEntry::SwitchSpeedVisibility_Implementation(bool bIsVisible)
{
	if (SpeedLabelTextBlock)
	{
		SpeedLabelTextBlock->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (SpeedDetailsTextBlock)
	{
		SpeedDetailsTextBlock->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonGenericModEntry::HandleSubscribeClicked_Implementation()
{
	SelectModListItem();

	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to subscribe/unsubscribe for mod entry '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	const FModioModID ModID = Execute_GetModID(this);
	if (!IsUserAuthenticated())
	{
		UE_LOG(ModioUI, Warning, TEXT("Unable to subscribe/unsubscribe for mod entry '%s' since user is not authenticated. Showing user auth dialog"), *GetName());
		Subsystem->ShowUserAuth();
		return;
	}

	if (!Execute_IsModSubscribed(this))
	{
		Subsystem->RequestSubscriptionForModID(ModID);
	}
	else
	{
		Subsystem->RequestRemoveSubscriptionForModID(ModID);
	}
}

void UModioCommonGenericModEntry::HandleSwitchEnabledClicked_Implementation()
{
	SelectModListItem();

	const FModioModID ModID = Execute_GetModID(this);
	RequestModEnabledState(ModID, !QueryModEnabled(ModID));
}

void UModioCommonGenericModEntry::HandleCancelClicked_Implementation()
{
	SelectModListItem();

	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to cancel subscription for mod entry '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	const FModioModID ModID = Execute_GetModID(this);
	if (!IsUserAuthenticated())
	{
		UE_LOG(ModioUI, Warning, TEXT("Unable to cancel subscription for mod entry '%s' since user is not authenticated. Showing user auth dialog"), *GetName());
		Subsystem->ShowUserAuth();
		return;
	}
	if (Execute_IsModSubscribed(this))
	{
		Subsystem->RequestRemoveSubscriptionForModID(ModID);
	}
	else
	{
		UE_LOG(ModioUI, Warning, TEXT("Unable to cancel subscription for mod entry '%s' since mod is not subscribed"), *GetName());
	}
}

void UModioCommonGenericModEntry::HandleOpenModDetailsClicked_Implementation()
{
	SelectModListItem();

	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to subscribe/unsubscribe for mod entry '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	const FModioModID CurrentModID = Execute_GetModID(this);
	Subsystem->ShowDetailsForMod(CurrentModID);
}

void UModioCommonGenericModEntry::HandleForceUninstallClicked_Implementation()
{
	if (!IsUserAuthenticated())
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>()) 
		{
			UE_LOG(ModioUI, Warning, TEXT("Unable to force uninstall for mod entry '%s' since user is not authenticated. Showing user auth dialog"), *GetName());
			Subsystem->ShowUserAuth();
			return;
		}
	}
	SelectModListItem();

	if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		const FModioModInfo CurrentModInfo = Execute_GetFullModInfo(this);
		if (UModioCommonDialogInfo* DialogInfo = UModioCommonDialogLibrary::CreateConfirmUninstallDialogInfo(CurrentModInfo))
		{
			ModioUISubsystem->ShowDialog(DialogInfo);
		}
	}
}

void UModioCommonGenericModEntry::HandlePrioritizeTransferClicked_Implementation()
{
	if (!IsUserAuthenticated())
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>()) 
		{
			UE_LOG(ModioUI, Warning, TEXT("Unable to prioritize transfer for mod entry '%s' since user is not authenticated. Showing user auth dialog"), *GetName());
			Subsystem->ShowUserAuth();
			return;
		}
	}
	SelectModListItem();
    
	if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		const FModioModID ModID = Execute_GetModID(this);
		ModioUISubsystem->PrioritizeTransferForMod(ModID);
	}
}