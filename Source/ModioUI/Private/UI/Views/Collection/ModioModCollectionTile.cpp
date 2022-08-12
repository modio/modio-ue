// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Views/Collection/ModioModCollectionTile.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "Core/ModioModInfoUI.h"
#include "Engine/Engine.h"
#include "Libraries/ModioSDKLibrary.h"
#include "ModioUISubsystem.h"
#include "UI/Commands/ModioCommonUICommands.h"
#include "UI/Interfaces/IModioUINotification.h"

#include "Loc/BeginModioLocNamespace.h"

void UModioModCollectionTile::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	UModioModCollectionEntryUI* CollectionEntry = Cast<UModioModCollectionEntryUI>(DataSource);
	if (CollectionEntry)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::InProgress);
			Subsystem->RequestLogoDownloadForModID(CollectionEntry->Underlying.GetModProfile().ModId);
		}
		if (ModName)
		{
			ModName->SetText(FText::FromString(CollectionEntry->Underlying.GetModProfile().ProfileName));
		}
		if (SizeOnDiskLabel)
		{
			const uint64 NumBytes = CollectionEntry->Underlying.GetSizeOnDisk().Underlying;
			if (NumBytes < GB)
			{
				SizeOnDiskLabel->SetText(UModioSDKLibrary::Filesize_ToString(NumBytes, 0));
			}
			else
			{
				SizeOnDiskLabel->SetText(UModioSDKLibrary::Filesize_ToString(NumBytes, 1));
			}
		}
		// Perform the initial hide of the remove mod button if we are not subscribed (because we want users to use
		// 'More Options' to do a force uninstall, in that case)
		if (SubscribeButton)
		{
			if (CollectionEntry->bCachedSubscriptionStatus == false)
			{
				SubscribeButton->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				SubscribeButton->SetVisibility(ESlateVisibility::Visible);
			}
		}
		if (SubscriptionIndicator)
		{
			SubscriptionIndicator->SetVisibility(CollectionEntry->bCachedSubscriptionStatus
													 ? ESlateVisibility::HitTestInvisible
													 : ESlateVisibility::Collapsed);
		}
		if (StatusWidget)
		{
			StatusWidget->SetDataSource(DataSource);
		}
		if (StatusLine)
		{
			StatusLine->SetText(CollectionEntry->bCachedSubscriptionStatus ? SubscribedStatusText
																		   : InstalledStatusText);
		}
		if (MoreOptionsMenu)
		{
			FModioUIMenuCommandList MenuEntries;
			FModioUIExecuteAction PositiveRatingDelegate;
			PositiveRatingDelegate.BindDynamic(this, &UModioModCollectionTile::SubmitPositiveRating);
			FModioUIExecuteAction NegativeRatingDelegate;
			NegativeRatingDelegate.BindDynamic(this, &UModioModCollectionTile::SubmitNegativeRating);
			FModioUIExecuteAction ReportDelegate;
			ReportDelegate.BindDynamic(this, &UModioModCollectionTile::SubmitModReport);

			MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("RateUp", "Rate Up")},
										  FModioUIAction {PositiveRatingDelegate});
			MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("RateDown", "Rate Down")},
										  FModioUIAction {NegativeRatingDelegate});
			MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("Report", "Report")},
										  FModioUIAction {ReportDelegate});

			// Force Uninstall should only be available if current user isn't subscribed
			if (CollectionEntry->bCachedSubscriptionStatus == false)
			{
				FModioUIExecuteAction ForceUninstallDelegate;
				ForceUninstallDelegate.BindDynamic(this, &UModioModCollectionTile::ForceUninstall);
				MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("ForceUninstall", "Force Uninstall")},
											  FModioUIAction {ForceUninstallDelegate});
			}
			MoreOptionsMenu->SetMenuEntries(MenuEntries);
		}
	}
}
FReply UModioModCollectionTile::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// TODO: @modio-ue4 should match our key mappings stuff
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource))
			{
				Subsystem->ShowDetailsForMod(ModInfo->Underlying.GetID());
				return FReply::Handled();
			}
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
void UModioModCollectionTile::NativeOnSetExpandedState(bool bExpanded)
{
	Super::NativeOnSetExpandedState(bExpanded);

	if (bExpanded) {}
}

void UModioModCollectionTile::NativeConstruct()
{
	Super::NativeConstruct();
	if (TileBorder)
	{
		TileBorder->ClearHoveredState();
	}
}

void UModioModCollectionTile::OnRatingSubmissionComplete(FModioErrorCode ec, EModioRating Rating)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->DisplayErrorNotification(UModioNotificationParamsLibrary::CreateRatingNotification(ec, DataSource));
	}
}
void UModioModCollectionTile::SubmitNegativeRating()
{
	UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			// Needs additional payload param so we know which type of operation was completed
			Subsystem->RequestRateDownForModId(
				ModInfo->Underlying.GetID(),
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioModCollectionTile::OnRatingSubmissionComplete,
														EModioRating::Negative));
		}
	}
}

void UModioModCollectionTile::SubmitPositiveRating()
{
	UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			// Needs additional payload param so we know which type of operation was completed
			Subsystem->RequestRateUpForModId(
				ModInfo->Underlying.GetID(),
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioModCollectionTile::OnRatingSubmissionComplete,
														EModioRating::Positive));
		}
	}
}

void UModioModCollectionTile::SubmitModReport()
{
	UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->ShowModReportDialog(ModInfo);
		}
	}
}

void UModioModCollectionTile::ForceUninstall()
{
	UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource);
	if (ModInfo)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (Subsystem)
			{
				Subsystem->ShowUninstallConfirmationDialog(ModInfo);
			}
		}
	}
}

void UModioModCollectionTile::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// Replace with interface registration call in NativePreConstruct();
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (Subsystem)
	{
		Subsystem->OnSubscriptionStatusChanged.AddUObject(this,
														  &UModioModCollectionTile::OnModSubscriptionStatusChanged);
	}
	if (TileBorder)
	{
		// TileBorder->SetHoveredState();
	}
}

/// We override this for ModCollectionTile specifically because it wraps a ModCollectionEntry, not a ModInfo
void UModioModCollectionTile::NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
															   TOptional<FModioImageWrapper> Image)
{
	IModioUIMediaDownloadCompletedReceiver::NativeOnModLogoDownloadCompleted(ModID, ec, Image);

	if (UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource))
	{
		if (ModID == ModInfo->Underlying.GetID())
		{
			if (ec)
			{
				UE_LOG(LogTemp, Error, TEXT("%s - %d"), *ec.GetErrorMessage(), ec.GetValue());
				IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																		   EModioUIAsyncOperationWidgetState::Error);
			}
			else
			{
				Thumbnail->LoadImageFromFileAsync(Image.GetValue());
				IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																		   EModioUIAsyncOperationWidgetState::Success);
			}
		}
	}
}

void UModioModCollectionTile::OnModSubscriptionStatusChanged(FModioModID ID, bool Subscribed)
{
	if (DataSource)
	{
		UModioModCollectionEntryUI* Data = Cast<UModioModCollectionEntryUI>(DataSource);
		if (Data)
		{
			if (Data->Underlying.GetModProfile().ModId == ID)
			{
				if (SubscribeButton)
				{
					// Show 'unsubscribe' button if we're displaying a mod the user is subscribed to
					SubscribeButton->SetVisibility(Subscribed ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
				}
				if (SubscriptionIndicator)
				{
					SubscriptionIndicator->SetVisibility(Subscribed ? ESlateVisibility::HitTestInvisible
																	: ESlateVisibility::Collapsed);
				}
				if (StatusLine)
				{
					StatusLine->SetText(Subscribed ? SubscribedStatusText : InstalledStatusText);
				}
			}
		}
	}
}

void UModioModCollectionTile::NativeOnItemSelectionChanged(bool bIsSelected)
{
	Super::NativeOnItemSelectionChanged(bIsSelected);
	if (bIsSelected)
	{
		// if (!bCurrentExpandedState)
		//{
		//	PlayAnimationForward(FocusTransition);
		//}
	}
	else
	{
		// PlayAnimationReverse(FocusTransition);
	}
}

void UModioModCollectionTile::NativeOnEntryReleased()
{
	// PlayAnimationReverse(FocusTransition);
	Super::NativeOnEntryReleased();
}

UModioModInfoUI* UModioModCollectionTile::GetAsModInfoUIObject()
{
	UModioModCollectionEntryUI* ModCollectionEntry = Cast<UModioModCollectionEntryUI>(DataSource);

	// We want to "convert" this UModioModCollectionEntryUI into a UModioModInfoUI with appropriate Underlying mod data
	// so we can pass it around our event system appropriately
	if (ModCollectionEntry)
	{
		UModioModInfoUI* ModInfo = NewObject<UModioModInfoUI>();

		ModInfo->Underlying = ModCollectionEntry->Underlying.GetModProfile();

		return ModInfo;
	}

	return nullptr;
}

void UModioModCollectionTile::BuildCommandList(TSharedRef<FUICommandList> CommandList)
{
	Super::BuildCommandList(CommandList);

	CommandList->MapAction(
		FModioCommonUICommands::Get().MoreOptions,
		FUIAction(FExecuteAction::CreateUObject(this, &UModioModCollectionTile::NativeMoreOptionsClicked)));
}

void UModioModCollectionTile::NativeMoreOptionsClicked()
{
	if (MoreOptionsMenu)
	{
		MoreOptionsMenu->ToggleOpen(true);
	}
}

#include "Loc/EndModioLocNamespace.h"