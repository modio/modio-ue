/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/Collection/ModioCollectionView.h"
#include "UI/Views/Collection/ModioModCollectionTile.h"
#include "UI/CommonComponents/ModioMenu.h"
#include "Algo/Transform.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "Core/Input/ModioInputKeys.h"
#include "Engine/Engine.h"
#include "Loc/BeginModioLocNamespace.h"
#include "ModioSubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioModCollectionEntry.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UI/Views/SearchResults/ModioSearchResultsView.h"

void UModioCollectionView::NativeOnInitialized()
{
	if (CollectionList)
	{
		OnFetchUpdatesClicked();
		ApplyFiltersToCollection();
	}

	if (ModGroupSelection)
	{
		ModGroupSelection->GetOnSelectionChanged().BindUObject(this, &UModioCollectionView::OnModGroupChanged);
	}
	if (FetchButton)
	{
		FetchButton->IsFocusable = true;
		FetchButton->SetLabel(DefaultButtonLabel);
		FetchButton->OnClicked.AddDynamic(this, &UModioCollectionView::OnFetchUpdatesClicked);
	}

	if (SortBy)
	{
		FModioUIMenuCommandList MenuEntries;

		FModioUIExecuteAction SortAToZDelegate;
		SortAToZDelegate.BindDynamic(this, &UModioCollectionView::SortAToZ);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("SortAToZ", "A->Z")},
									  FModioUIAction {SortAToZDelegate});

		FModioUIExecuteAction SortZToADelegate;
		SortZToADelegate.BindDynamic(this, &UModioCollectionView::SortZToA);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("SortZToA", "Z->A")},
									  FModioUIAction {SortZToADelegate});

		FModioUIExecuteAction SortSizeOnDiskDelegate;
		SortSizeOnDiskDelegate.BindDynamic(this, &UModioCollectionView::SortSizeOnDisk);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("SortSizeOnDisk", "Size On Disk")},
									  FModioUIAction {SortSizeOnDiskDelegate});

		FModioUIExecuteAction SortRecentlyUpdatedDelegate;
		SortRecentlyUpdatedDelegate.BindDynamic(this, &UModioCollectionView::SortRecentlyUpdatedDelegate);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("SortRecentlyUpdated", "Recently Updated")},
									  FModioUIAction {SortRecentlyUpdatedDelegate});

		SortBy->SetOnSelectionChangedWithDelegate(TSlateDelegates<FModioUIAction>::FOnSelectionChanged::CreateUObject(
			this, &UModioCollectionView::OnSelectionChanged));

		SortBy->SetComboBoxEntries(MenuEntries);
	}

	IModioUISubscriptionsChangedReceiver::Register<UModioCollectionView>();
	IModioUIModManagementEventReceiver::Register<UModioCollectionView>();
}

void UModioCollectionView::OnFetchExternalCompleted(FModioErrorCode ec)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->DisplayNotificationParams(UModioNotificationParamsLibrary::CreateNotificationParams(
			ec, LOCTEXT("FetchUpdatesSucceeded", "Updates fetched!"), LOCTEXT("FetchUpdatesSucceeded", "Updates fetched!"),
			LOCTEXT("FetchUpdatesFailed", "Could not fetch updates.")));
	}
	if (!ec)
	{
		UpdateCachedCollection();
		ApplyFiltersToCollection();

		// focus is lost after search probably because collection list items are not quite there yet when getting here, so
		// setting the focus to searchinput:
		SearchInput->StartInput();
	}
	else
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			
			// Subsystem->DisplayNotification(LOCTEXT("FetchUpdatesFailed", "Fetch updates failed. Please retry"));
		}
	}

	FetchButton->SetLabel(DefaultButtonLabel);
}

void UModioCollectionView::UpdateCachedCollection()
{
	CachedCollection.Empty();
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		TMap<FModioModID, FModioModCollectionEntry> UserSubscriptions = Subsystem->QueryUserSubscriptions();
		TMap<FModioModID, FModioModCollectionEntry> SystemInstallations = Subsystem->QuerySystemInstallations();
		Algo::Transform(SystemInstallations, CachedCollection,
						[&UserSubscriptions](const TPair<FModioModID, FModioModCollectionEntry>& In) {
							UModioModCollectionEntryUI* WrappedModCollectionEntry =
								NewObject<UModioModCollectionEntryUI>();
							WrappedModCollectionEntry->Underlying = In.Value;
							WrappedModCollectionEntry->bCachedSubscriptionStatus = UserSubscriptions.Contains(In.Key);
							return WrappedModCollectionEntry;
						});
		CollectionList->SetListItems(CachedCollection);
		CollectionList->RegenerateAllEntries();
	}
}

void UModioCollectionView::OnFetchUpdatesClicked()
{
	// TODO: @modio-ue4 Currently nothing to display an error to the user, can investigate that possibility later
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->FetchExternalUpdatesAsync(
			FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioCollectionView::OnFetchExternalCompleted));
		FetchButton->SetLabel(SearchingButtonLabel);
	}
}

void UModioCollectionView::ApplyFiltersToCollection()
{
	FilteredCollection = CachedCollection.FilterByPredicate([this](UModioModCollectionEntryUI* CurrentEntry) {
		// if the user only wants user subscriptions, reject anything that the user isn't subscribed to
		if (ModGroupSelection)
		{
			// Bad to have a hardcoded index, but because we don't actually have an enum for user, vs system
			// subscriptions, there's a fixed order of potential choices
			if (ModGroupSelection->GetSelectedItemIndex() == 0)
			{
				if (CurrentEntry->bCachedSubscriptionStatus == false)
				{
					return false;
				}
			}
		}

		// If the user has provided non-empty filter text, check the name for the search string
		if (SearchInput && !SearchInput->GetText().IsEmptyOrWhitespace())
		{
			if (!CurrentEntry->Underlying.GetModProfile().ProfileName.Contains(SearchInput->GetText().ToString()))
			{
				return false;
			}
		}
		return true;
	});

	if (HasSortActionApplied)
	{
		CurrentSortAction.ExecuteAction.ExecuteIfBound();
	}
	else
	{
		SortAToZ();
	}
	UpdateModCount();
}

void UModioCollectionView::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	UpdateCachedCollection();
	ApplyFiltersToCollection();
	// to prevent weird focus jumping:
	ValidateAndSetFocus();
}

void UModioCollectionView::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);
	UpdateCachedCollection();
	ApplyFiltersToCollection();
	// to prevent weird focus jumping:
	ValidateAndSetFocus();
}

void UModioCollectionView::NativeConstruct()
{
	Super::NativeConstruct();
	CollectionList->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UModioCollectionView::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (ModGroupSelection)
	{
		TArray<FText> ModGroupStrings;
		ModGroupStrings.Add(ShowUserSubscriptionsText);
		ModGroupStrings.Add(ShowSystemCollectionText);
		ModGroupSelection->SetOptionValues(ModGroupStrings);
		ModGroupSelection->SetSelectedItemIndex(0);
	}
}

void UModioCollectionView::OnSelectionChanged(FModioUIAction ModioUIAction, ESelectInfo::Type Arg)
{
	CurrentSortAction = ModioUIAction;
	HasSortActionApplied = true;
	
	CurrentSortAction.ExecuteAction.ExecuteIfBound();
}

void UModioCollectionView::SortAToZ()
{
	Algo::Sort(FilteredCollection, [](UModioModCollectionEntryUI* A, UModioModCollectionEntryUI* B) {
		return FName(A->Underlying.GetModProfile().ProfileName)
			.LexicalLess(FName(B->Underlying.GetModProfile().ProfileName));
	});

	CollectionList->SetListItems(FilteredCollection);
	CollectionList->RegenerateAllEntries();
}

void UModioCollectionView::SortRecentlyUpdatedDelegate()
{
	Algo::Sort(FilteredCollection, [](UModioModCollectionEntryUI* A, UModioModCollectionEntryUI* B) {
		return A->Underlying.GetModProfile().ProfileDateUpdated < B->Underlying.GetModProfile().ProfileDateUpdated;
	});

	CollectionList->SetListItems(FilteredCollection);
	CollectionList->RegenerateAllEntries();
}

void UModioCollectionView::SortSizeOnDisk()
{
	Algo::Sort(FilteredCollection, [](UModioModCollectionEntryUI* A, UModioModCollectionEntryUI* B) {
		return A->Underlying.GetSizeOnDisk() > B->Underlying.GetSizeOnDisk();
	});

	CollectionList->SetListItems(FilteredCollection);
	CollectionList->RegenerateAllEntries();
}

void UModioCollectionView::SortZToA()
{
	Algo::Sort(FilteredCollection, [](UModioModCollectionEntryUI* A, UModioModCollectionEntryUI* B) {
		return FName(B->Underlying.GetModProfile().ProfileName)
			.LexicalLess(FName(A->Underlying.GetModProfile().ProfileName));
	});

	CollectionList->SetListItems(FilteredCollection);
	CollectionList->RegenerateAllEntries();
}

void UModioCollectionView::OnModGroupChanged(FText SelectedItem, ESelectInfo::Type SelectionType)
{
	ApplyFiltersToCollection();
}

void UModioCollectionView::UpdateModCount()
{
	if (CollectionCount)
	{
		int numberOfVisibleMods = CollectionList->GetNumItems();
		for (auto& item : CollectionList->GetListItems())
		{
			UModioModCollectionEntryUI* entry = Cast<UModioModCollectionEntryUI>(item);
			if (entry->Underlying.GetModState() != EModioModState::Installed &&
				!entry->bCachedSubscriptionStatus)
			{
				numberOfVisibleMods--;
			}
		}
		FString Count = FString::Printf(TEXT("(%d)"), numberOfVisibleMods);
		CollectionCount->SetText(FText::FromString(Count));
		if (numberOfVisibleMods <= 0)
		{
			InfoRichTextBlock->SetVisibility(ESlateVisibility::Visible);
			InfoRichTextBlock->SetText(CachedCollection.Num() > 0 ? NoModsFoundText : NoSubscribedModsText);
		}
		else
		{
			InfoRichTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UModioCollectionView::ValidateAndSetFocus()
{
	if (CurrentNavIndex < CollectionList->GetNumItems() && CurrentNavIndex >= 0) 
	{
		CollectionList->NavigateToIndex(CurrentNavIndex);
		CollectionList->SetSelectedIndex(CurrentNavIndex);
		CurrentTile = Cast<UModioModCollectionTile>(CollectionList->GetEntryWidgetFromItem(CollectionList->GetSelectedItem()));
	}
	else
	{
		CurrentNavIndex = -1;
		GetWorld()->GetTimerManager().SetTimerForNextTick(SearchInput, &UModioEditableTextBox::StartInput);
	}
}

FReply UModioCollectionView::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();

	if (CollectionList->GetNumItems() > 0 && IsValid(Subsystem) && !(Subsystem->GetLastInputDevice() == EModioUIInputMode::Mouse))
	{
		ValidateAndSetFocus();
	}

	else
	{
		CurrentNavIndex = -1;
		GetWorld()->GetTimerManager().SetTimerForNextTick(SearchInput, &UModioEditableTextBox::StartInput);
	}

	return FReply::Handled();
}
bool UModioCollectionView::ValidateSearchInput(const FKeyEvent& InKeyEvent)
{
	return InKeyEvent.GetKey() != EKeys::Up && InKeyEvent.GetKey() != EKeys::Down &&
		   InKeyEvent.GetKey() != EKeys::Left && InKeyEvent.GetKey() != EKeys::Right &&
		   InKeyEvent.GetKey() != EKeys::Escape;
}

FReply UModioCollectionView::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (SearchInput->HasFocusedDescendants() && ValidateSearchInput(InKeyEvent))
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UModioCollectionView::ApplyFiltersToCollection);

		return FReply::Unhandled();
	}
	
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();

	if (!IsValid(Subsystem))
	{
		return FReply::Handled();
	}

	UModioMenu* Menu = Cast <UModioMenu>(Subsystem->ModBrowserInstance);

	if (IsValid(Menu) && Menu->IsAnyDrawerExpanded())
	{
		return FReply::Handled();
	}

	UModioUI4Subsystem* UI4Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();

	if (IsValid(UI4Subsystem) && UI4Subsystem->IsAnyDialogOpen())
	{
		return FReply::Handled();
	}

	if (ProcessCommandForEvent(InKeyEvent))
	{
		return FReply::Handled();
	}

	if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::DownloadQueue && CurrentNavIndex != 0)
	{
		OnProfileOpened.Broadcast();
		return FReply::Handled();
	}

	if (Subsystem->IsDownloadDrawerOpen())
	{
		return FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Enter && SearchInput->HasFocusedDescendants())
	{
		OnFetchUpdatesClicked();
		return FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Tab && CurrentNavIndex == 0)
	{
		ValidateAndSetFocus();
		return FReply::Handled();
	}

	if (!HasFocusedDescendants()) 
	{
		SearchInput->StartInput();
		return FReply::Handled();
	}

	if (IsValid(Subsystem))
	{
		
		if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::RefineSearch && (!SearchInput->HasFocusedDescendants() || InKeyEvent.GetKey().IsGamepadKey()))
		{
			OnFetchUpdatesClicked();
			return FReply::Handled();
		}

		if (CollectionList->GetDisplayedEntryWidgets().IsValidIndex(0))
		{
			int currentIndex = CollectionList->GetIndexForItem(CollectionList->GetSelectedItem());
			if (CurrentNavIndex != currentIndex && currentIndex >= 0)
			{
				CurrentNavIndex = currentIndex;
			}
			if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Down)
			{
				if (CurrentTile && CurrentTile->MoreOptionsMenu->GetIsMenuOpen())
				{
					return FReply::Unhandled();
				}
				else if ((SortBy->HasFocusedDescendants() && !SortBy->IsComboBoxOpen()) ||
					(ModGroupSelection->HasFocusedDescendants() && !ModGroupSelection->IsComboBoxOpen()) || SearchInput->HasFocusedDescendants())
				{
					CurrentNavIndex = 0;
					ValidateAndSetFocus();
					return FReply::Handled();
				}
				else if (CurrentNavIndex < CollectionList->GetNumItems() - 1 &&
						 !ModGroupSelection->IsComboBoxOpen() && !SortBy->IsComboBoxOpen())
				{
					CurrentNavIndex++;
					ValidateAndSetFocus();
					return FReply::Handled();
				}
			}

			if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Up)
			{
				if (CurrentTile && CurrentTile->MoreOptionsMenu->GetIsMenuOpen())
				{
					return FReply::Unhandled();
				}
				else if (CurrentNavIndex == 0 && !SearchInput->HasFocusedDescendants() && !ModGroupSelection->HasFocusedDescendants() && !SortBy->HasFocusedDescendants())
				{
					SearchInput->StartInput();
					return FReply::Handled();
				}
				else if (CollectionList->HasFocusedDescendants())
				{
					CurrentNavIndex--;
					ValidateAndSetFocus();
					return FReply::Handled();
				}
			}
			
		}
	}
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

#include "Loc/EndModioLocNamespace.h"