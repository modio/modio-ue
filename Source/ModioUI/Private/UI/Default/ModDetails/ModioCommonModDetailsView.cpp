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
#include "ModioUI.h"
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
#include "UI/Default/Dialog/ModioCommonDialogInfo.h"
#include <TimerManager.h>
#include "Libraries/ModioErrorConditionLibrary.h"
#include "UI/Default/ErrorWithRetry/ModioCommonErrorWithRetryWidget.h"
#include "UI/Interfaces/IModioUIAsyncHandlerWidget.h"
#include "UI/Settings/ModioCommonUISettings.h"

UModioCommonModDetailsView::UModioCommonModDetailsView()
{
	bAutoBindInputAction = false;
}

void UModioCommonModDetailsView::SetStyle(TSubclassOf<UModioCommonModDetailsViewStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
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

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>()) 
	{
		if (FileSizeLabelTextBlock)
		{
			FileSizeLabelTextBlock->SetText(UISettings->ModDetailsParams.FileSizeLabel);
		}

		if (LastUpdatedLabelTextBlock)
		{
			LastUpdatedLabelTextBlock->SetText(UISettings->ModDetailsParams.LastUpdatedLabel);
		}

		if (ReleaseDateLabelTextBlock)
		{
			ReleaseDateLabelTextBlock->SetText(UISettings->ModDetailsParams.ReleaseDateLabel);
		}

		if (SubscribersLabelTextBlock)
		{
			SubscribersLabelTextBlock->SetText(UISettings->ModDetailsParams.SubscribersLabel);
		}

		if (CreatedByLabelTextBlock)
		{
			CreatedByLabelTextBlock->SetText(UISettings->ModDetailsParams.CreatedByLabel);
		}

		if (TagsLabelTextBlock)
		{
			TagsLabelTextBlock->SetText(UISettings->ModDetailsParams.TagsLabel);
		}

		if (ModFullDescriptionLabelTextBlock)
		{
			ModFullDescriptionLabelTextBlock->SetText(UISettings->ModDetailsParams.ModDescriptionLabel);
		}
	}
}

void UModioCommonModDetailsView::NativeOnModEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	Super::NativeOnModEnabledStateChanged(ModID, bNewSubscriptionState);
	const FModioModID CurrentModID = Execute_GetModID(this);

	if (DataSource && CurrentModID == ModID)
	{
		SetDataSource(DataSource);
		UpdateInputActions();
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

	if (CollectionButton) 
	{
		CollectionButton->OnClicked.AddDynamic(this, &UModioCommonModDetailsView::HandleCollectionClicked);
	}

	if (TagsWidget)
	{
		TagsWidget->OnFocusChangedFast.AddWeakLambda(this, [this](bool bIsFocused, UObject*)
		{
			if (bIsFocused)
			{
				ActivateTagsInputBindings();
			}
		});
	}

	if (Execute_IsModDownloading(this) || Execute_IsModExtracting(this))
	{
		ShowProgress();
	}
	else
	{
		HideProgress();
	}

	if (Execute_IsModInQueue(this))
	{
		ShowStatus();
	}

	if (ErrorWithRetryWidget)
	{
		ErrorWithRetryWidget->RetryClickedDelegate.AddWeakLambda(this, [this]() {
			SetDataSource(DataSource);
		});
	}

	if (ModDetailsLoader && ModDetailsLoader->Implements<UModioUIAsyncHandlerWidget>())
	{
		TScriptInterface<IModioUIAsyncOperationWidget> InterfaceWrapper = TScriptInterface<IModioUIAsyncOperationWidget>(this);
		IModioUIAsyncHandlerWidget::Execute_LinkAsyncOperationWidget(ModDetailsLoader, InterfaceWrapper);
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
				if (Event.Event == EModioModManagementEventType::Installed) 
				{ 
					ShowStatus(); 
				}
				
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

	if (ModDetailsLoader && ModDetailsLoader->Implements<UModioUIAsyncHandlerWidget>()
		&& IModioUIAsyncHandlerWidget::Execute_GetAsyncOperationState(ModDetailsLoader) == EModioUIAsyncOperationWidgetState::Error)
	{
		if (UWidget* WidgetToFocus = ErrorWithRetryWidget->GetDesiredFocusTarget())
		{
			return WidgetToFocus;
		}
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

void UModioCommonModDetailsView::ActivateTopButtonsInputBindings_Implementation()
{
	ClearListeningInputActions();
	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>()) 
	{
		if (SubscribeButton && SubscribeButton->IsVisible())
		{
			ListenForInputAction(SubscribeButton, UISettings->ModDetailsParams.SubscribeInputAction, Execute_IsModSubscribed(this) ? UISettings->ModDetailsParams.UnsubscribeLabel : UISettings->ModDetailsParams.SubscribeLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]()
			{
				HandleSubscribeClicked();
			}));
		}
		else if (CancelButton && CancelButton->IsVisible())
		{
			ListenForInputAction(CancelButton, UISettings->ModDetailsParams.CancelInputAction, UISettings->ModDetailsParams.CancelLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]()
			{
				HandleCancelClicked();
			}));
		}

		if (ReportButton)
		{
			ListenForInputAction(ReportButton, UISettings->ModDetailsParams.OpenReportInputAction, UISettings->ModDetailsParams.ReportLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]()
			{
				HandleReportClicked();
			}));
		}

		const bool bModInstalled = Execute_IsModInstalled(this);
		const bool bModEnabled = Execute_IsModEnabled(this);

		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (bModInstalled && Subsystem->GetIsCollectionModDisableUIEnabled())
			{
				SwitchEnableButtonVisibility(!bModEnabled);
				SwitchDisableButtonVisibility(bModEnabled);

				UModioCommonButtonBase* EnableDisableButton = bModEnabled ? DisableButton : EnableButton;
				if (EnableDisableButton)
				{
					ListenForInputAction(EnableDisableButton, UISettings->ModDetailsParams.SwitchEnabledInputAction, bModEnabled ? UISettings->ModDetailsParams.DisableLabel : UISettings->ModDetailsParams.EnableLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
						HandleSwitchEnabledClicked();
					}));
				}
			}
			else
			{
				SwitchEnableButtonVisibility(false);
				SwitchDisableButtonVisibility(false);
			}
		}
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to activate top buttons input bindings for '%s': Settings are invalid"), *GetName());
	}
	UnbindInputActions();
	BindInputActions();
}

void UModioCommonModDetailsView::ActivateBottomButtonsInputBindings_Implementation()
{
	ClearListeningInputActions();
	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>()) 
	{
		if (RateUpButton)
		{
			ListenForInputAction(RateUpButton, UISettings->ModDetailsParams.RateUpInputAction, UISettings->ModDetailsParams.RateUpLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]()
			{
				HandleRateUpClicked();
			}));
		}
		if (RateDownButton)
		{
			ListenForInputAction(RateDownButton, UISettings->ModDetailsParams.RateDownInputAction, UISettings->ModDetailsParams.RateDownLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]()
			{
				HandleRateDownClicked();
			}));
		}
		if (ReportButton)
		{
			ListenForInputAction(ReportButton, UISettings->ModDetailsParams.OpenReportInputAction, UISettings->ModDetailsParams.ReportLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]()
			{
				HandleReportClicked();
			}));
		}
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to activate bottom buttons input bindings for '%s': Settings are invalid"), *GetName());
	}
	UnbindInputActions();
	BindInputActions();
}

void UModioCommonModDetailsView::ActivateTagsInputBindings_Implementation()
{
	ClearListeningInputActions();
	UnbindInputActions();
	BindInputActions();
}

void UModioCommonModDetailsView::UpdateInputActions_Implementation()
{
	if (TagsWidget->HasFocusedDescendants())
	{
		ClearListeningInputActions();
	}
	else if ((RateUpButton && RateUpButton->IsButtonFocused())
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

void UModioCommonModDetailsView::SwitchEnableButtonVisibility_Implementation(bool bIsVisible)
{
	if (EnableButton)
	{
		EnableButton->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonModDetailsView::SwitchDisableButtonVisibility_Implementation(bool bIsVisible)
{
	if (DisableButton)
	{
		DisableButton->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
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

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>()) 
	{
#if WITH_EDITOR
		if (OperationProgressLabelTextBlock && IsDesignTime())
		{
			OperationProgressLabelTextBlock->SetText(UISettings->ModDetailsParams.DownloadingLabel);
		}
#endif

		if (OperationProgressLabelTextBlock)
		{
			if (Execute_IsModDownloading(this))
			{
				OperationProgressLabelTextBlock->SetText(UISettings->ModDetailsParams.DownloadingLabel);
			}
			else if (Execute_IsModExtracting(this))
			{
				OperationProgressLabelTextBlock->SetText(UISettings->ModDetailsParams.ExtractingLabel);
			}
			else
			{
				OperationProgressLabelTextBlock->SetText(FText::GetEmpty());
			}
		}
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to update operation progress for '%s': Settings are invalid"), *GetName());
	}
}

void UModioCommonModDetailsView::UpdateOperationProgressBytes_Implementation(FModioUnsigned64 Current, FModioUnsigned64 Total)
{
	
}

void UModioCommonModDetailsView::UpdateSpeed_Implementation(FModioUnsigned64 DeltaBytes, double DeltaTime)
{
	if (SpeedDetailsTextBlock)
	{
		SpeedDetailsTextBlock->SetVisibility(Execute_IsModDownloading(this) ? ESlateVisibility::Visible :  ESlateVisibility::Hidden);
	}
	
	if (SpeedDetailsTextBlock)
	{
		const int64 BytesPerSecond = DeltaBytes.Underlying <= 0 ? 0 : DeltaBytes / DeltaTime / 8;
		const FText TotalText = UModioSDKLibrary::Filesize_ToString(BytesPerSecond, 1, 1);
		SpeedDetailsTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s/s"), *TotalText.ToString())));
	}
}

void UModioCommonModDetailsView::OnRatingSubmissionComplete_Implementation(FModioErrorCode ErrorCode, EModioRating SubmittedRating)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (ErrorCode)
		{
			Subsystem->DisplayErrorDialog(ErrorCode);
			IsRateLimited(ErrorCode);
		}
		else
		{
			Subsystem->DisplayNotificationParams(UModioNotificationParamsLibrary::CreateRatingNotification(ErrorCode, DataSource.Get()));
		}
	}
	AllowRatingUp(true);
	AllowRatingDown(true);
}

void UModioCommonModDetailsView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::InProgress);

	if (!DataSource)
	{
		IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::Error);
		return;
	}

	const FModioModInfo ModInfo = Execute_GetFullModInfo(this);

	if (ModGalleryView)
	{
		ModGalleryView->SetDataSource(DataSource);
	}

	if (ModNameTextBlock)
	{
		ModNameTextBlock->SetText(FText::FromString(ModInfo.ProfileName));
	}

	if (ModFullDescriptionTextBlock)
	{
		ModFullDescriptionTextBlock->SetText(FText::FromString(!ModInfo.ProfileDescriptionPlaintext.IsEmpty() ? ModInfo.ProfileDescriptionPlaintext : ModInfo.ProfileSummary));
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
		LastUpdatedTextBlock->SetText(FText::FromString(UModioSDKLibrary::GetTimeSpanAsString(ModInfo.ProfileDateUpdated.ToString())));
	}

	if (ReleaseDateTextBlock)
	{
		ReleaseDateTextBlock->SetText(FText::FromString(UModioSDKLibrary::GetTimeSpanAsString(ModInfo.ProfileDateLive.ToString())));
	}

	if (SubscribersTextBlock)
	{
		SubscribersTextBlock->SetText(FText::FromString(FString::FormatAsNumber(ModInfo.Stats.SubscribersTotal)));
	}

	if (CreatedByTextBlock)
	{
		CreatedByTextBlock->SetText(FText::FromString(ModInfo.ProfileSubmittedBy.Username));
	}

	if (RateUpButton)
	{
		RateUpButton->SetLabel(FText::FromString(FString::FormatAsNumber(ModInfo.Stats.RatingPositive)));
	}

	if (RateDownButton)
	{
		RateDownButton->SetLabel(FText::FromString(FString::FormatAsNumber(ModInfo.Stats.RatingNegative)));
	}

	if (TagsWidget)
	{
		TagsWidget->SetTags(ModInfo.Tags);
	}

	if (InstalledCheckBox) 
	{
		InstalledCheckBox->SetCheckedState(Execute_IsModInstalled(this) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	}

	if (EnabledCheckBox) 
	{
		EnabledCheckBox->SetCheckedState(Execute_IsModEnabled(this) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	}

	if (Execute_IsModDownloading(this) || Execute_IsModExtracting(this))
	{
		ShowProgress();
	}
	else
	{
		HideProgress();
		if (Execute_IsModSubscribed(this) || Execute_IsModInQueue(this))
		{
			ShowStatus();
		}
	}

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>()) 
	{
		if (SubscribeButton)
		{
			SubscribeButton->SetLabel(Execute_IsModSubscribed(this) ? UISettings->ModDetailsParams.UnsubscribeLabel : UISettings->ModDetailsParams.SubscribeLabel);
		}

		if (EnableButton)
		{
			EnableButton->SetLabel(UISettings->ModDetailsParams.EnableLabel);
		}

		if (DisableButton)
		{
			DisableButton->SetLabel(UISettings->ModDetailsParams.DisableLabel);
		}
	}

	IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::Success);
	FocusOnDesiredWidget();
}

bool UModioCommonModDetailsView::Initialize()
{
	const bool bSuperInitialized = Super::Initialize();

	if(UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		RateDownButton->SetVisibility(ESlateVisibility::Collapsed);
		ModioUISubsystem->GetGameInfoAsync(FOnGetGameInfoDelegateFast::CreateWeakLambda(this, [this](FModioErrorCode ErrorCode, TOptional<FModioGameInfo> GameInfo) {
			if(!ErrorCode && GameInfo.IsSet())
			{
				RateDownButton->SetVisibility(GameInfo.GetValue().bAllowNegativeRatings ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			}
		 }));
	}
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

	if (StatusContainer)
	{
		StatusContainer->SetVisibility(ESlateVisibility::Collapsed);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			StatusContainer->SetRenderOpacity(0);
		}
#endif
	}

	if (ProgressBarContainer)
	{
		ProgressBarContainer->SetVisibility(ESlateVisibility::Visible);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			ProgressBarContainer->SetRenderOpacity(1);
		}
#endif
	}

	if (OperationContainer)
	{
		OperationContainer->SetVisibility(ESlateVisibility::Visible);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			OperationContainer->SetRenderOpacity(1);
		}
#endif
	}

	if (CancelButton) 
	{
		CancelButton->SetVisibility(ESlateVisibility::Visible);

		#if WITH_EDITOR
		if (IsDesignTime())
		{
			CancelButton->SetRenderOpacity(1);
		}
#endif
	}
}

void UModioCommonModDetailsView::ShowStatus_Implementation()
{
	if (ProgressBarContainer)
	{
		ProgressBarContainer->SetVisibility(ESlateVisibility::Collapsed);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			ProgressBarContainer->SetRenderOpacity(0);
		}
#endif
	}

	if (InstalledCheckBox) 
	{
		InstalledCheckBox->SetCheckedState(Execute_IsModInstalled(this) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	}

	if (EnabledCheckBox) 
	{
		EnabledCheckBox->SetCheckedState(Execute_IsModEnabled(this) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	}

	if (StatusContainer)
	{
		StatusContainer->SetVisibility(ESlateVisibility::Visible);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			StatusContainer->SetRenderOpacity(1);
		}
#endif
	}

	if (OperationContainer)
	{
		OperationContainer->SetVisibility(ESlateVisibility::Visible);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			OperationContainer->SetRenderOpacity(1);
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

	if (OperationContainer)
	{
		OperationContainer->SetVisibility(ESlateVisibility::Collapsed);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			OperationContainer->SetRenderOpacity(0);
		}
#endif
	}

	if (CancelButton)
	{
		CancelButton->SetVisibility(ESlateVisibility::Collapsed);

#if WITH_EDITOR
		if (IsDesignTime())
		{
			CancelButton->SetRenderOpacity(0);
		}
#endif
	}
}

void UModioCommonModDetailsView::HandleSubscribeClicked_Implementation()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to subscribe/unsubscribe for mod details '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	if (!IsUserAuthenticated())
	{
		UE_LOG(ModioUI, Warning, TEXT("Unable to subscribe/unsubscribe for mod details '%s' since user is not authenticated. Showing user auth dialog"), *GetName());
		Subsystem->ShowUserAuth();
		return;
	}

	IsRateLimited(Subsystem->LastSubscriptionErrorCode);

	const FModioModID ModID = Execute_GetModID(this);
	
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
		UE_LOG(ModioUI, Error, TEXT("Unable to cancel subscription for mod entry '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	if (!IsUserAuthenticated())
	{
		UE_LOG(ModioUI, Warning, TEXT("Unable to cancel subscription for mod entry '%s' since user is not authenticated. Showing user auth dialog"), *GetName());
		Subsystem->ShowUserAuth();
		return;
	}
	const FModioModID ModID = Execute_GetModID(this);
	if (Execute_IsModSubscribed(this))
	{
		Subsystem->RequestRemoveSubscriptionForModID(ModID);
	}
	else
	{
		UE_LOG(ModioUI, Warning, TEXT("Unable to cancel subscription for mod entry '%s' since mod is not subscribed"), *GetName());
	}
}

void UModioCommonModDetailsView::HandleRateUpClicked_Implementation()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to rate up a mod for mod details '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	if (IsUserAuthenticated()) 
	{
		AllowRatingUp(false);
	}
	const FModioModID CurrentModID = Execute_GetModID(this);
	Subsystem->RequestRateUpForModId(CurrentModID, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioCommonModDetailsView::OnRatingSubmissionComplete, EModioRating::Positive));
}

void UModioCommonModDetailsView::HandleRateDownClicked_Implementation()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to rate down a mod for mod details '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	if (IsUserAuthenticated())
	{
		AllowRatingDown(false);
	}
	const FModioModID CurrentModID = Execute_GetModID(this);
	Subsystem->RequestRateDownForModId(CurrentModID, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioCommonModDetailsView::OnRatingSubmissionComplete, EModioRating::Negative));
}

void UModioCommonModDetailsView::HandleReportClicked_Implementation()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to report a mod for mod details '%s': Modio UI Subsystem is invalid"), *GetName());
		return;
	}

	const FModioModInfo CurrentModInfo = Execute_GetFullModInfo(this);
	UModioModInfoUI* ModInfoObj = NewObject<UModioModInfoUI>();
	ModInfoObj->Underlying = CurrentModInfo;
	Subsystem->ShowModReportDialog(ModInfoObj);
}

void UModioCommonModDetailsView::HandleCollectionClicked_Implementation() 
{
	if(UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioCommonModBrowser* ModBrowser = Cast<UModioCommonModBrowser>(UISubsystem->GetModBrowserInstance()))
		{
			ModBrowser->ShowCollectionView();
		}
	}
}

void UModioCommonModDetailsView::AllowRatingUp(bool bAllow) 
{
	if (RateUpButton) 
	{
		RateUpButton->SetIsEnabled(bAllow);
	}
}

void UModioCommonModDetailsView::AllowRatingDown(bool bAllow)
{
	if (RateDownButton)
	{
		RateDownButton->SetIsEnabled(bAllow);
	}
}

bool UModioCommonModDetailsView::IsRateLimited(FModioErrorCode ErrorCode)
{
	if (UModioErrorConditionLibrary::ErrorCodeMatches(ErrorCode, EModioErrorCondition::RateLimited) ||
		UModioErrorConditionLibrary::ErrorCodeMatches(ErrorCode, EModioErrorCondition::ModBeingProcessed))
	{
		if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			const FModioModInfo CurrentModInfo = Execute_GetFullModInfo(this);
			if (UModioCommonDialogInfo* DialogInfo = UModioCommonDialogLibrary::CreateErrorDialogInfo(ErrorCode, FText::FromString(CurrentModInfo.ProfileName)))
			{
				ModioUISubsystem->ShowDialog(DialogInfo);
			}
			return true;
		}
	}
	return false;
}
