// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Views/CategoryBrowser/ModioFeaturedMod.h"
#include "Core/ModioModInfoUI.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"

#include "Loc/BeginModioLocNamespace.h"

void UModioFeaturedMod::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (DataSource)
	{
		UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
		if (ModInfo)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			Subsystem->RequestLogoDownloadForModID(ModInfo->Underlying.ModId);
			if (ModName)
			{
				ModName->SetText(FText::FromString(ModInfo->Underlying.ProfileName));
			}
			// Some of this logic perhaps would be better on the Subscribe button itself, treat that button as a special
			// case
			/*if (SubscribeButton)
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
			}*/

			if (SubscriptionIndicator)
			{
				SubscriptionIndicator->SetDataSource(DataSource);
			}
		}
	}
}

void UModioFeaturedMod::SubmitNegativeRating()
{
	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (ModInfo)
	{
		UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
		if (Subsystem)
		{
			// Needs additional payload param so we know which type of operation was completed
			Subsystem->SubmitModRatingAsync(
				ModInfo->Underlying.ModId, EModioRating::Negative,
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioFeaturedMod::OnRatingSubmissionComplete,
														EModioRating::Negative));
		}
	}
}

void UModioFeaturedMod::SubmitPositiveRating()
{
	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (ModInfo)
	{
		UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
		if (Subsystem)
		{
			// Needs additional payload param so we know which type of operation was completed
			Subsystem->SubmitModRatingAsync(
				ModInfo->Underlying.ModId, EModioRating::Positive,
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioFeaturedMod::OnRatingSubmissionComplete,
														EModioRating::Positive));
		}
	}
}

void UModioFeaturedMod::OnRatingSubmissionComplete(FModioErrorCode ec, EModioRating Rating)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->DisplayErrorNotification(UModioNotificationParamsLibrary::CreateRatingNotification(ec, DataSource));
	}
}

void UModioFeaturedMod::SubmitModReport()
{
	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->ShowModReportDialog(ModInfo);
		}
	}
}
void UModioFeaturedMod::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (MoreOptionsMenu)
	{
		FModioUIMenuCommandList MenuEntries;
		FModioUIExecuteAction PositiveRatingDelegate;
		// TODO: move these to static UFUNCTIONs someplace?
		PositiveRatingDelegate.BindDynamic(this, &UModioFeaturedMod::SubmitPositiveRating);
		FModioUIExecuteAction NegativeRatingDelegate;
		NegativeRatingDelegate.BindDynamic(this, &UModioFeaturedMod::SubmitNegativeRating);
		FModioUIExecuteAction ReportDelegate;
		ReportDelegate.BindDynamic(this, &UModioFeaturedMod::SubmitModReport);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("RateUp", "Rate Up")},
									  FModioUIAction {PositiveRatingDelegate});
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("RateDown", "Rate Down")},
									  FModioUIAction {NegativeRatingDelegate});
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("Report", "Report")}, FModioUIAction {ReportDelegate});

		MoreOptionsMenu->SetMenuEntries(MenuEntries);
	}
}

// This should be 'while in the focus path'
FReply UModioFeaturedMod::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (FocusTransition)
	{
		PlayAnimation(FocusTransition);
	}
	// hide/show the subscriptionbutton moreoptions and modname
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UModioFeaturedMod::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	if (FocusTransition)
	{
		PlayAnimationReverse(FocusTransition);
	}
}

void UModioFeaturedMod::NativeOnItemSelectionChanged(bool bIsSelected)
{
	Super::NativeOnItemSelectionChanged(bIsSelected);
	bCurrentSelectionState = bIsSelected;
	if (SubscribeButton)
	{
		SubscribeButton->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (ModName)
	{
		ModName->SetVisibility(bIsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
	if (MoreOptionsMenu)
	{
		MoreOptionsMenu->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (TileActiveFrame)
	{
		TileActiveFrame->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UModioFeaturedMod::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
}

void UModioFeaturedMod::NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
														 TOptional<FModioImageWrapper> Image)
{
	Super::NativeOnModLogoDownloadCompleted(ModID, ec, Image);
}

FReply UModioFeaturedMod::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bCurrentSelectionState)
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}
	return FReply::Unhandled();
}

#include "Loc/EndModioLocNamespace.h"