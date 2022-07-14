#include "UI/Views/ModDetails/ModioModDetailsView.h"
#include "Engine/Engine.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Loc/BeginModioLocNamespace.h"
#include "ModioSubsystem.h"
#include "Types/ModioRating.h"
#include "UI/Commands/ModioCommonUICommands.h"
#include "UI/EventHandlers/IModioUIAuthenticationChangedReceiver.h"
#include "UI/Interfaces/IModioUIAsyncHandlerWidget.h"
#include "Widgets/Notifications/SNotificationList.h"

void UModioModDetailsView::NativeConstruct()
{
	Super::NativeConstruct();
	IModioUIModDetailsDisplay::Register<UModioModDetailsView>();
}

void UModioModDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (DetailsLoader)
	{
		TScriptInterface<IModioUIAsyncOperationWidget> InterfaceWrapper =
			TScriptInterface<IModioUIAsyncOperationWidget>(this);
		IModioUIAsyncHandlerWidget::Execute_LinkAsyncOperationWidget(DetailsLoader, InterfaceWrapper);
	}

	if (RateUpButton)
	{
		RateUpButton->OnClicked.AddDynamic(this, &UModioModDetailsView::RateUpClicked);
	}

	if (RateDownButton)
	{
		RateDownButton->OnClicked.AddDynamic(this, &UModioModDetailsView::RateDownClicked);
	}

	if (ReportButton)
	{
		ReportButton->OnClicked.AddDynamic(this, &UModioModDetailsView::ReportClicked);
	}

	if (DetailsViewContent)
	{
		DetailsViewContent->SetActualAsyncOperationWidget(TScriptInterface<IModioUIAsyncOperationWidget>(this));
	}

	IModioUIModInfoReceiver::Register<UModioModDetailsView>(EModioUIModInfoEventType::GetModInfo);

	bCachedSubscriptionState = false;

	if (SubscribeButton)
	{
		SubscribeButton->OnClicked.AddDynamic(this, &UModioModDetailsView::NativeSubscribeClicked);
	}

	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (Subsystem)
	{
		Subsystem->OnSubscriptionStatusChanged.AddUObject(this, &UModioModDetailsView::OnModSubscriptionStatusChanged);
	}
}

void UModioModDetailsView::OnModSubscriptionStatusChanged(FModioModID ID, bool Subscribed)
{
	if (DataSource)
	{
		UModioModInfoUI* Data = Cast<UModioModInfoUI>(DataSource);
		if (Data)
		{
			if (Data->Underlying.ModId == ID)
			{
				if (SubscribeButton)
				{
					// Show 'unsubscribe' text if we're displaying a mod the user is subscribed to
					SubscribeButton->SetLabel(Subscribed ? UnsubscribeLabel : SubscribeLabel);
				}
				if (SubscriptionBadge)
				{
					SubscriptionBadge->SetVisibility(Subscribed ? ESlateVisibility::HitTestInvisible
																: ESlateVisibility::Collapsed);
				}
				bCachedSubscriptionState = Subscribed;
			}
		}
	}
}

void UModioModDetailsView::NativeSubscribeClicked()
{
	if (!bCachedSubscriptionState)
	{
		UModioModInfoUI* Data = Cast<UModioModInfoUI>(DataSource);
		if (Data)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->RequestSubscriptionForModID(Data->Underlying.ModId);
			}
		}
	}
	else
	{
		UModioModInfoUI* Data = Cast<UModioModInfoUI>(DataSource);
		if (Data)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->ShowModUnsubscribeDialog(Data);
				;
			}
		}
	}
}

void UModioModDetailsView::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (SubscribeButton)
	{
		SubscribeButton->SetLabel(SubscribeButtonFormat);
	}
	// this will need to pull values from the data source when we have one set

	if (RateUpButton)
	{
		RateUpButton->SetLabel(RateUpTextFormat);
	}
	if (RateDownButton)
	{
		RateDownButton->SetLabel(RateDownTextFormat);
	}
	if (ReportButton)
	{
		ReportButton->SetLabel(ReportTextFormat);
	}

#if WITH_EDITOR
	// This could likely be moved elsewhere, but as it is editor-only I'm not too worried
	if (EditorPreviewDataSource && IsDesignTime())
	{
		SetDataSource(EditorPreviewDataSource);
	}
#endif
}

void UModioModDetailsView::NativeDisplayModDetails(const TScriptInterface<class IModioModInfoUIDetails>& Details)
{
	// SetDataSource(NewDataSource);
}

void UModioModDetailsView::NativeDisplayModDetailsForId(const FModioModID& ModID)
{
	ShowDetailsForMod(ModID);
}

void UModioModDetailsView::NativeRequestOperationRetry()
{
	ShowDetailsForMod(CurrentModID);
}

// These methods potentially need us to know about the user's ratings
void UModioModDetailsView::RateUpClicked()
{
	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			// Needs additional payload param so we know which type of operation was completed
			Subsystem->RequestRateUpForModId(
				ModInfo->Underlying.ModId,
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioModDetailsView::OnRatingSubmissionComplete,
														EModioRating::Positive));
		}
	}
}

void UModioModDetailsView::RateDownClicked()
{
	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			// Needs additional payload param so we know which type of operation was completed
			Subsystem->RequestRateDownForModId(
				ModInfo->Underlying.ModId,
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioModDetailsView::OnRatingSubmissionComplete,
														EModioRating::Negative));
		}
	}
}

void UModioModDetailsView::ReportClicked()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (Subsystem)
	{
		Subsystem->ShowModReportDialog(DataSource);
	}
}

void UModioModDetailsView::OnRatingSubmissionComplete(FModioErrorCode ec, EModioRating SubmittedRating)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->DisplayErrorNotification(UModioNotificationParamsLibrary::CreateRatingNotification(ec, DataSource));
	}

	if (!ec)
	{
		switch (SubmittedRating)
		{
			case EModioRating::Positive:
			case EModioRating::Negative:
			{
			}
			break;
			case EModioRating::Neutral:
				// ensure that neither button is highlighted
				break;
			default:
				break;
		}
	}
	else
	{}
}

void UModioModDetailsView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (ModInfo)
	{
		if (ModTitleTextBlock)
		{
			ModTitleTextBlock->SetText(FText::FromString(ModInfo->Underlying.ProfileName));
		}

		if (ModSummaryTextBlock)
		{
			ModSummaryTextBlock->SetText(FText::FromString(ModInfo->Underlying.ProfileSummary));
		}
		if (ModChangelog)
		{
			ModChangelog->SetText(FText::FromString(ModInfo->Underlying.FileInfo.Changelog));
		}

		if (SubscriptionBadge)
		{
			SubscriptionBadge->SetDataSource(DataSource);
		}

		if (ProgressWidget)
		{
			ProgressWidget->SetDataSource(DataSource);
		}

		if (ImageGallery)
		{
			ImageGallery->SetDataSource(DataSource);
			ImageGallery->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (ModFullDescriptionTextBlock)
		{
			ModFullDescriptionTextBlock->SetText(FText::FromString(ModInfo->Underlying.ProfileDescriptionPlaintext));
		}

		if (ModTags)
		{
			ModTags->SetTags(ModInfo->Underlying.Tags);
		}

		FFormatNamedArguments FormatArgs;
		FormatArgs.Add("RatingPositive", ModInfo->Underlying.Stats.RatingPositive);
		FormatArgs.Add("RatingNegative", ModInfo->Underlying.Stats.RatingNegative);
		if (RateUpButton)
		{
			RateUpButton->SetLabel(FText::Format(RateUpTextFormat, FormatArgs));
		}
		if (RateDownButton)
		{
			RateDownButton->SetLabel(FText::Format(RateDownTextFormat, FormatArgs));
		}

		if (SubscribeButton)
		{
			SubscribeButton->SetLabel(SubscribeLabel);

			if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
			{
				// Can hit this function directly because the primary subsystem caches the subscriptions, so we dont
				// have to do it manually in the UI subsystem
				if (Subsystem->QueryUserSubscriptions().Contains(ModInfo->Underlying.ModId))
				{
					bCachedSubscriptionState = true;
					SubscribeButton->SetLabel(UnsubscribeLabel);
				}
			}
		}
		if (ModPropertiesInspector)
		{
			ModPropertiesInspector->SetDataSource(DataSource);
		}
		if (ProgressWidget)
		{
			ProgressWidget->SetDataSource(DataSource);
		}
	}
}

void UModioModDetailsView::NativeOnModInfoRequestCompleted(FModioModID ModID, FModioErrorCode ec,
														   TOptional<FModioModInfo> Info)
{
	IModioUIModInfoReceiver::NativeOnModInfoRequestCompleted(ModID, ec, Info);

	if (CurrentModID == ModID)
	{
		if (!ec)
		{
			if (UModioModInfoUI* NewDataSource = NewObject<UModioModInfoUI>())
			{
				NewDataSource->Underlying = Info.GetValue();
				SetDataSource(NewDataSource);
			}
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::Success);
		}
		else
		{
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::Error);
		}
	}
}

void UModioModDetailsView::ShowDetailsForMod(FModioModID ID)
{
	CurrentModID = ID;
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->RequestModInfoForModIDs({ID});
		IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::InProgress);
	}
	if (ImageGallery)
	{
		ImageGallery->SetVisibility(ESlateVisibility::Hidden);
	}
}

FReply UModioModDetailsView::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	FSlateApplication::Get().SetUserFocus(0, SubscribeButton->TakeWidget(), EFocusCause::Navigation);

	return FReply::Handled();
}

#include "Loc/EndModioLocNamespace.h"