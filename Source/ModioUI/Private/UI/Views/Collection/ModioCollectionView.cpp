#include "UI/Views/Collection/ModioCollectionView.h"
#include "Algo/Transform.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "Engine/Engine.h"
#include "Loc/BeginModioLocNamespace.h"
#include "ModioSubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioModCollectionEntry.h"
#include "UI/Views/SearchResults/ModioSearchResultsView.h"

void UModioCollectionView::NativeOnInitialized()
{
	if (CollectionList)
	{
		UpdateCachedCollection();
		ApplyFiltersToCollection();
	}

	if (SearchInput)
	{
		SearchInput->OnTextChanged.AddDynamic(this, &UModioCollectionView::OnSearchTextUpdated);
	}

	if (ModGroupSelection)
	{
		ModGroupSelection->GetOnSelectionChanged().BindUObject(this, &UModioCollectionView::OnModGroupChanged);
	}
	if (FetchButton)
	{
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
		Subsystem->DisplayErrorNotification(UModioNotificationParamsLibrary::CreateErrorNotificationParams(
			ec, LOCTEXT("FetchUpdatesSucceeded", "Updates fetched!"),
			LOCTEXT("FetchUpdatesFailed", "Could not fetch updates.")));
	}
	if (!ec)
	{
		UpdateCachedCollection();
		ApplyFiltersToCollection();
	}
	else
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			// Subsystem->DisplayNotification(LOCTEXT("FetchUpdatesFailed", "Fetch updates failed. Please retry"));
		}
	}
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

		if (CollectionCount)
		{
			FString Count = FString::Printf(TEXT("(%d)"), CachedCollection.Num());
			CollectionCount->SetText(FText::FromString(Count));
		}
	}
}

void UModioCollectionView::OnFetchUpdatesClicked()
{
	// TODO: @modio-ue4 Currently nothing to display an error to the user, can investigate that possibility later
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->FetchExternalUpdatesAsync(
			FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioCollectionView::OnFetchExternalCompleted));
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
}

void UModioCollectionView::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	UpdateCachedCollection();
	ApplyFiltersToCollection();
}

void UModioCollectionView::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);
	UpdateCachedCollection();
	ApplyFiltersToCollection();
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

void UModioCollectionView::OnSearchTextUpdated(const FText& NewText)
{
	ApplyFiltersToCollection();
}

void UModioCollectionView::OnModGroupChanged(FText SelectedItem, ESelectInfo::Type SelectionType)
{
	ApplyFiltersToCollection();
}

#include "Loc/EndModioLocNamespace.h"