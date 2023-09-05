/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/ModDetails/ModioCommonModDetailsView.h"

#include "Components/HorizontalBox.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "Core/ModioModInfoUI.h"
#include "ModioSubsystem.h"
#include "ModioUI5.h"
#include "Layout/Visibility.h"
#include "Libraries/ModioSDKLibrary.h"
#include "UI/Default/Gallery/ModioCommonModGalleryView.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Settings/Params/ModioCommonModDetailsParams.h"
#include "UI/Foundation/Components/ProgressBar/ModioCommonProgressBar.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerWidget.h"
#include "UI/Default/ModDetails/ModioCommonModDetailsViewStyle.h"
#include "UI/Foundation/Components/Tag/ModioCommonModTagList.h"
#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerUserWidget.h"

UModioCommonModDetailsView::UModioCommonModDetailsView()
{
	bAutoBindInputAction = false;
}

void UModioCommonModDetailsView::SetStyle(TSubclassOf<UModioCommonModDetailsViewStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

void UModioCommonModDetailsView::NativePreConstruct()
{
	Super::NativePreConstruct();

#if WITH_EDITOR
	if (IsDesignTime())
	{
		if (UModioModInfoUI* ModInfo = NewObject<UModioModInfoUI>())
		{
			ModInfo->Underlying = PreviewModInfo;
			SetDataSource(ModInfo);
		}
	}
#endif
}

void UModioCommonModDetailsView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (UModioCommonModDetailsViewStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (ModGalleryView)
		{
			ModGalleryView->SetStyle(StyleCDO->ModGalleryViewStyle);
		}
		
		if (ModNameTextBlock)
		{
			ModNameTextBlock->SetStyle(StyleCDO->ModNameTextStyle);
		}
		
		if (SubscribeButton)
		{
			SubscribeButton->SetStyle(StyleCDO->SubscribeButtonStyle);
		}

		if (CancelButton)
		{
			CancelButton->SetStyle(StyleCDO->CancelButtonStyle);
		}

		if (RateUpButton)
		{
			RateUpButton->SetStyle(StyleCDO->RateUpButtonStyle);
		}

		if (RateDownButton)
		{
			RateDownButton->SetStyle(StyleCDO->RateDownButtonStyle);
		}

		if (ReportButton)
		{
			ReportButton->SetStyle(StyleCDO->ReportButtonStyle);
		}

		if (ModSummaryTextBlock)
		{
			ModSummaryTextBlock->SetStyle(StyleCDO->ModSummaryTextStyle);
		}

		if (ModFullDescriptionLabelTextBlock)
		{
			ModFullDescriptionLabelTextBlock->SetStyle(StyleCDO->ModFullDescriptionLabelTextStyle);
		}

		if (ModFullDescriptionTextBlock)
		{
			ModFullDescriptionTextBlock->SetStyle(StyleCDO->ModFullDescriptionTextStyle);
		}

		if (OperationProgressBar)
		{
			OperationProgressBar->SetStyle(StyleCDO->DownloadProgressBarStyle);
		}

		if (OperationProgressLabelTextBlock)
		{
			OperationProgressLabelTextBlock->SetStyle(StyleCDO->DownloadProgressLabelTextStyle);
		}

		if (OperationProgressTextBlock)
		{
			OperationProgressTextBlock->SetStyle(StyleCDO->DownloadProgressTextStyle);
		}

		if (FileSizeLabelTextBlock)
		{
			FileSizeLabelTextBlock->SetStyle(StyleCDO->StatsLabelTextStyle);
		}

		if (FileSizeTextBlock)
		{
			FileSizeTextBlock->SetStyle(StyleCDO->StatsTextStyle);
		}

		if (LastUpdatedLabelTextBlock)
		{
			LastUpdatedLabelTextBlock->SetStyle(StyleCDO->StatsLabelTextStyle);
		}

		if (LastUpdatedTextBlock)
		{
			LastUpdatedTextBlock->SetStyle(StyleCDO->StatsTextStyle);
		}

		if (ReleaseDateLabelTextBlock)
		{
			ReleaseDateLabelTextBlock->SetStyle(StyleCDO->StatsLabelTextStyle);
		}

		if (ReleaseDateTextBlock)
		{
			ReleaseDateTextBlock->SetStyle(StyleCDO->StatsTextStyle);
		}

		if (SubscribersLabelTextBlock)
		{
			SubscribersLabelTextBlock->SetStyle(StyleCDO->StatsLabelTextStyle);
		}

		if (SubscribersTextBlock)
		{
			SubscribersTextBlock->SetStyle(StyleCDO->StatsTextStyle);
		}

		if (CreatedByLabelTextBlock)
		{
			CreatedByLabelTextBlock->SetStyle(StyleCDO->StatsLabelTextStyle);
		}

		if (CreatedByTextBlock)
		{
			CreatedByTextBlock->SetStyle(StyleCDO->StatsTextStyle);
		}

		if (TagsLabelTextBlock)
		{
			TagsLabelTextBlock->SetStyle(StyleCDO->StatsLabelTextStyle);
		}
	}

	if (const UModioCommonModDetailsParamsSettings* Settings = GetDefault<UModioCommonModDetailsParamsSettings>())
	{
		if (FileSizeLabelTextBlock)
		{
			FileSizeLabelTextBlock->SetText(Settings->FileSizeLabel);
		}

		if (LastUpdatedLabelTextBlock)
		{
			LastUpdatedLabelTextBlock->SetText(Settings->LastUpdatedLabel);
		}

		if (ReleaseDateLabelTextBlock)
		{
			ReleaseDateLabelTextBlock->SetText(Settings->ReleaseDateLabel);
		}

		if (SubscribersLabelTextBlock)
		{
			SubscribersLabelTextBlock->SetText(Settings->SubscribersLabel);
		}

		if (CreatedByLabelTextBlock)
		{
			CreatedByLabelTextBlock->SetText(Settings->CreatedByLabel);
		}

		if (TagsLabelTextBlock)
		{
			TagsLabelTextBlock->SetText(Settings->TagsLabel);
		}

		if (ModFullDescriptionLabelTextBlock)
		{
			ModFullDescriptionLabelTextBlock->SetText(Settings->ModDescriptionLabel);
		}
	}
}

void UModioCommonModDetailsView::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	UpdateInputActions();
}

void UModioCommonModDetailsView::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	ClearListeningInputActions();
}

void UModioCommonModDetailsView::NativeOnInitialized() 
{
	Super::NativeOnInitialized();
	IModioUIModManagementEventReceiver::Register<UModioCommonModDetailsViewBase>();

	if (SubscribeButton)
	{
		SubscribeButton->OnFocusReceived().AddWeakLambda(this, [this]()
		{
			ActivateTopButtonsInputBindings();
		});
	}

	if (CancelButton)
	{
		CancelButton->OnFocusReceived().AddWeakLambda(this, [this]()
		{
			ActivateTopButtonsInputBindings();
		});
	}

	if (RateUpButton)
	{
		RateUpButton->OnFocusReceived().AddWeakLambda(this, [this]()
		{
			ActivateBottomButtonsInputBindings();
		});
	}

	if (RateDownButton)
	{
		RateDownButton->OnFocusReceived().AddWeakLambda(this, [this]()
		{
			ActivateBottomButtonsInputBindings();
		});
	}

	if (ReportButton)
	{
		ReportButton->OnFocusReceived().AddWeakLambda(this, [this]()
		{
			ActivateBottomButtonsInputBindings();
		});
	}
}

void UModioCommonModDetailsView::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	Super::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	if (Execute_GetModID(this) == ModID)
	{
		SetDataSource(DataSource);
	}
}

void UModioCommonModDetailsView::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	Super::NativeOnModManagementEvent(Event);

	if (Execute_GetModID(this) == Event.ID)
	{
		switch (Event.Event) {
			case EModioModManagementEventType::Installed:
			case EModioModManagementEventType::Uninstalled:
			{
				HideProgress();
				UpdateInputActions();
				FocusOnDesiredWidget();
				break;
			}
			case EModioModManagementEventType::BeginInstall:
			case EModioModManagementEventType::BeginUninstall:
			{
				UpdateOperationProgressPercent(0.f);
				ShowProgress();
				UpdateInputActions();
				FocusOnDesiredWidget();
				break;
			}
		}
	}
}

UWidget* UModioCommonModDetailsView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (RateUpButton && RateUpButton->IsButtonFocused())
	{
		return RateUpButton;
	}
	if (RateDownButton && RateDownButton->IsButtonFocused())
	{
		return RateDownButton;
	}
	if (ReportButton && ReportButton->IsButtonFocused())
	{
		return ReportButton;
	}

	if (SubscribeButton && SubscribeButton->IsVisible())
	{
		return SubscribeButton;
	}
	if (CancelButton && CancelButton->IsVisible())
	{
		return CancelButton;
	}

	if (SubscribeButton && SubscribeButton->IsButtonFocused())
	{
		return SubscribeButton;
	}
	if (CancelButton && CancelButton->IsButtonFocused())
	{
		return CancelButton;
	}
	
	return SubscribeButton;
}

void UModioCommonModDetailsView::ActivateTopButtonsInputBindings()
{
	ClearListeningInputActions();
	if (const UModioCommonModDetailsParamsSettings* Settings = GetDefault<UModioCommonModDetailsParamsSettings>())
	{
		if (SubscribeButton && SubscribeButton->IsVisible())
		{
			ListenForInputAction(SubscribeButton, Settings->SubscribeInputAction, Execute_IsModSubscribed(this) ? Settings->UnsubscribeLabel : Settings->SubscribeLabel, [this]()
			{
				HandleSubscribeClicked();
			});
		}
		else if (CancelButton && CancelButton->IsVisible())
		{
			ListenForInputAction(CancelButton, Settings->CancelInputAction, Settings->CancelLabel, [this]()
			{
				HandleCancelClicked();
			});
		}
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to activate top buttons input bindings for '%s': Settings are invalid"), *GetName());
	}
	UnbindInputActions();
	BindInputActions();
}

void UModioCommonModDetailsView::ActivateBottomButtonsInputBindings()
{
	ClearListeningInputActions();
	if (const UModioCommonModDetailsParamsSettings* Settings = GetDefault<UModioCommonModDetailsParamsSettings>())
	{
		ListenForInputAction(RateUpButton, Settings->RateUpInputAction, Settings->RateUpLabel, [this]()
		{
			HandleRateUpClicked();
		});
		ListenForInputAction(RateDownButton, Settings->RateDownInputAction, Settings->RateDownLabel, [this]()
		{
			HandleRateDownClicked();
		});
		ListenForInputAction(ReportButton, Settings->OpenReportInputAction, Settings->ReportLabel, [this]()
		{
			HandleReportClicked();
		});
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to activate bottom buttons input bindings for '%s': Settings are invalid"), *GetName());
	}
	UnbindInputActions();
	BindInputActions();
}

void UModioCommonModDetailsView::UpdateInputActions_Implementation()
{
	if ((RateUpButton && RateUpButton->IsButtonFocused())
		|| (RateDownButton && RateDownButton->IsButtonFocused())
		|| (ReportButton && ReportButton->IsButtonFocused()))
	{
		ActivateBottomButtonsInputBindings();
	}
	else
	{
		ActivateTopButtonsInputBindings();
	}
}

void UModioCommonModDetailsView::UpdateOperationProgressPercent_Implementation(float InPercent)
{
	if (OperationProgressBar)
	{
		OperationProgressBar->SetPercent(InPercent);
	}

	if (OperationProgressTextBlock)
	{
		const int32 Percentage = InPercent * 100;
		OperationProgressTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), Percentage)));
	}

	if (const UModioCommonModDetailsParamsSettings* Settings = GetDefault<UModioCommonModDetailsParamsSettings>())
	{
#if WITH_EDITOR
		if (IsDesignTime())
		{
			OperationProgressLabelTextBlock->SetText(Settings->DownloadingLabel);
		}
#endif

		if (OperationProgressLabelTextBlock)
		{
			if (Execute_IsModDownloading(this))
			{
				OperationProgressLabelTextBlock->SetText(Settings->DownloadingLabel);
			}
			else if (Execute_IsModExtracting(this))
			{
				OperationProgressLabelTextBlock->SetText(Settings->ExtractingLabel);
			}
			else
			{
				OperationProgressLabelTextBlock->SetText(FText::GetEmpty());
			}
		}
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to update operation progress for '%s': Settings are invalid"), *GetName());
	}
}

void UModioCommonModDetailsView::UpdateOperationProgressBytes_Implementation(FModioUnsigned64 Current, FModioUnsigned64 Total)
{
	
}

void UModioCommonModDetailsView::UpdateSpeed_Implementation(FModioUnsigned64 DeltaBytes, double DeltaTime)
{
	
}

void UModioCommonModDetailsView::OnRatingSubmissionComplete_Implementation(FModioErrorCode ErrorCode, EModioRating SubmittedRating)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (ErrorCode)
		{
			Subsystem->DisplayErrorDialog(ErrorCode);
		}
		else
		{
			Subsystem->DisplayNotificationParams(UModioNotificationParamsLibrary::CreateRatingNotification(ErrorCode, DataSource.Get()));
		}
	}
}

void UModioCommonModDetailsView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	const FModioModInfo ModInfo = Execute_GetFullModInfo(this);

	if (ModGalleryView)
	{
		ModGalleryView->LoadGallery(ModInfo);
	}

	if (ModNameTextBlock)
	{
		ModNameTextBlock->SetText(FText::FromString(ModInfo.ProfileName));
	}

	if (ModFullDescriptionTextBlock)
	{
		ModFullDescriptionTextBlock->SetText(FText::FromString(ModInfo.ProfileDescriptionPlaintext));
	}

	if (ModSummaryTextBlock)
	{
		ModSummaryTextBlock->SetText(FText::FromString(ModInfo.ProfileSummary));
	}

	if (FileSizeTextBlock)
	{
		FileSizeTextBlock->SetText(UModioSDKLibrary::Filesize_ToString(ModInfo.FileInfo.Filesize, 0, 2));
	}

	if (LastUpdatedTextBlock)
	{
		LastUpdatedTextBlock->SetText(FText::FromString(ModInfo.ProfileDateUpdated.ToString()));
	}

	if (ReleaseDateTextBlock)
	{
		ReleaseDateTextBlock->SetText(FText::FromString(ModInfo.ProfileDateLive.ToString()));
	}

	if (SubscribersTextBlock)
	{
		SubscribersTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%lld"), ModInfo.Stats.SubscribersTotal)));
	}

	if (CreatedByTextBlock)
	{
		CreatedByTextBlock->SetText(FText::FromString(ModInfo.ProfileSubmittedBy.Username));
	}

	if (RateUpButton)
	{
		RateUpButton->SetLabel(FText::FromString(FString::Printf(TEXT("%lld"), ModInfo.Stats.RatingPositive)));
	}

	if (RateDownButton)
	{
		RateDownButton->SetLabel(FText::FromString(FString::Printf(TEXT("%lld"), ModInfo.Stats.RatingNegative)));
	}

	if (TagsWidget)
	{
		TagsWidget->SetTags(ModInfo.Tags);
	}

	if (const UModioCommonModDetailsParamsSettings* Settings = GetDefault<UModioCommonModDetailsParamsSettings>())
	{
		if (SubscribeButton)
		{
			SubscribeButton->SetLabel(Execute_IsModSubscribed(this) ? Settings->UnsubscribeLabel : Settings->SubscribeLabel);
		}
	}

	if (Execute_IsModDownloading(this) || Execute_IsModExtracting(this))
	{
		ShowProgress();
	}
	else
	{
		HideProgress();
	}
}

bool UModioCommonModDetailsView::Initialize()
{
	const bool bSuperInitialized = Super::Initialize();
	if (ModOperationTrackerWidget)
	{
		ModOperationTrackerWidget->OnProgressFast.RemoveAll(this);
		ModOperationTrackerWidget->OnProgressFast.AddWeakLambda(this, [this](FModioUnsigned64 Current, FModioUnsigned64 Total) {
			if (Total)
			{
				ShowProgress();
				UpdateOperationProgressPercent(Total ? (Current / static_cast<double>(Total)) : 0);
				UpdateOperationProgressBytes(Current, Total);
			}
		});

		ModOperationTrackerWidget->OnSpeedFast.RemoveAll(this);
		ModOperationTrackerWidget->OnSpeedFast.AddWeakLambda(this, [this](FModioUnsigned64 DeltaBytes, double DeltaTime) {
			if (DeltaBytes)
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

void UModioCommonModDetailsView::ShowProgress_Implementation()
{
	if (SubscribeButton)
	{
		SubscribeButton->SetVisibility(ESlateVisibility::Collapsed);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			SubscribeButton->SetRenderOpacity(0);
		}
#endif
	}

	if (OperationProgressBarContainer)
	{
		OperationProgressBarContainer->SetVisibility(ESlateVisibility::Visible);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			OperationProgressBarContainer->SetRenderOpacity(1);
		}
#endif
	}
}

void UModioCommonModDetailsView::HideProgress_Implementation()
{
	if (SubscribeButton)
	{
		SubscribeButton->SetVisibility(ESlateVisibility::Visible);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			SubscribeButton->SetRenderOpacity(1);
		}
#endif
	}

	if (OperationProgressBarContainer)
	{
		OperationProgressBarContainer->SetVisibility(ESlateVisibility::Collapsed);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			OperationProgressBarContainer->SetRenderOpacity(0);
		}
#endif
	}
}

void UModioCommonModDetailsView::HandleSubscribeClicked_Implementation()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to subscribe/unsubscribe for mod details '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	const FModioModID ModID = Execute_GetModID(this);

	if (!IsUserAuthenticated())
	{
		UE_LOG(ModioUI5, Warning, TEXT("Unable to subscribe/unsubscribe for mod details '%s' since user is not authenticated. Showing user auth dialog"), *GetName());
		if (Subsystem->ModBrowserInstance->Implements<UModioModBrowserInterface>())
		{
			IModioModBrowserInterface::Execute_ShowUserAuth(Subsystem->ModBrowserInstance);
		}
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

void UModioCommonModDetailsView::HandleSwitchEnabledClicked_Implementation()
{
	const FModioModID ModID = Execute_GetModID(this);
	RequestModEnabledState(ModID, !QueryModEnabled(ModID));
}

void UModioCommonModDetailsView::HandleCancelClicked_Implementation()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to cancel subscription for mod entry '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	const FModioModID ModID = Execute_GetModID(this);
	if (!IsUserAuthenticated())
	{
		UE_LOG(ModioUI5, Warning, TEXT("Unable to cancel subscription for mod entry '%s' since user is not authenticated. Showing user auth dialog"), *GetName());
		if (Subsystem->ModBrowserInstance->Implements<UModioModBrowserInterface>())
		{
			IModioModBrowserInterface::Execute_ShowUserAuth(Subsystem->ModBrowserInstance);
		}
		return;
	}
	if (Execute_IsModSubscribed(this))
	{
		Subsystem->RequestRemoveSubscriptionForModID(ModID);
	}
	else
	{
		UE_LOG(ModioUI5, Warning, TEXT("Unable to cancel subscription for mod entry '%s' since mod is not subscribed"), *GetName());
	}
}

void UModioCommonModDetailsView::HandleRateUpClicked_Implementation()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to rate up a mod for mod details '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	const FModioModID CurrentModID = Execute_GetModID(this);
	Subsystem->RequestRateUpForModId(CurrentModID, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioCommonModDetailsView::OnRatingSubmissionComplete, EModioRating::Positive));
}

void UModioCommonModDetailsView::HandleRateDownClicked_Implementation()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to rate down a mod for mod details '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	const FModioModID CurrentModID = Execute_GetModID(this);
	Subsystem->RequestRateDownForModId(CurrentModID, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioCommonModDetailsView::OnRatingSubmissionComplete, EModioRating::Negative));
}

void UModioCommonModDetailsView::HandleReportClicked_Implementation()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to report a mod for mod details '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	const FModioModInfo CurrentModInfo = Execute_GetFullModInfo(this);
	UModioModInfoUI* ModInfoObj = NewObject<UModioModInfoUI>();
	ModInfoObj->Underlying = CurrentModInfo;
	Subsystem->ShowModReportDialog(ModInfoObj);
}
