
/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/ModBrowser/Collection/ModioCommonCollectionView.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "Loc/BeginModioLocNamespace.h"
#include "UI/Default/ModBrowser/Collection/ModioCommonCollectionViewStyle.h"
#include "UI/Foundation/Components/List/ModioCommonModListView.h"
#include "UI/Foundation/Components/List/ModioCommonModTileView.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "Components/ListView.h"
#include "Algo/Reverse.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "TimerManager.h"
#include "ModioUI.h"
#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"
#include "UI/Settings/ModioCommonUISettings.h"

void UModioCommonCollectionView::SetStyle(TSubclassOf<UModioCommonCollectionViewStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonCollectionView::UpdateMods_Implementation()
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to update mods in '%s': Modio Subsystem is not available"), *GetName());
		return;
	}

	UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!UISubsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to update mods in '%s': Modio UI Subsystem is not available"), *GetName());
		return;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& Installations = Subsystem->QuerySystemInstallations();
	const TMap<FModioModID, FModioModCollectionEntry>& UserSubscriptions = Subsystem->QueryUserSubscriptions();

	// Rebuild the Pending Downloads list
	TArray<FModioModCollectionEntry> QueuedDownloadingEntries;
	UserSubscriptions.GenerateValueArray(QueuedDownloadingEntries);

	TArray<FModioModCollectionEntry> QueuedEntries = QueuedDownloadingEntries;
	QueuedEntries.RemoveAll([](const FModioModCollectionEntry& Entry) {
		return Entry.GetModState() != EModioModState::UpdatePending
		       && Entry.GetModState() != EModioModState::InstallationPending
		       && Entry.GetModState() != EModioModState::UninstallPending;
	});

	TArray<FModioModCollectionEntry> DownloadingEntries = QueuedDownloadingEntries;
	DownloadingEntries.RemoveAll([](const FModioModCollectionEntry& Entry) {
		return Entry.GetModState() != EModioModState::Downloading
			   && Entry.GetModState() != EModioModState::Extracting;
	});

	TArray<FModioModCollectionEntry> InstalledEntries;
	Installations.GenerateValueArray(InstalledEntries);
	InstalledEntries.RemoveAll([](const FModioModCollectionEntry& Entry) {
		return Entry.GetModState() != EModioModState::Installed;
	});
	
	const int32 NumOfInstalledMods = InstalledEntries.Num();

	ApplySortingAndFiltering(DownloadingEntries);
	ApplySortingAndFiltering(QueuedEntries);
	ApplySortingAndFiltering(InstalledEntries);

	TArray<FModioModCollectionEntry> Entries;
	Entries.Reserve(DownloadingEntries.Num() + QueuedEntries.Num() + InstalledEntries.Num());
	Entries.Append(MoveTemp(DownloadingEntries));
	Entries.Append(MoveTemp(QueuedEntries));
	Entries.Append(MoveTemp(InstalledEntries));

	if (NumOfErrorsTextBlock)
	{
		NumOfErrorsTextBlock->SetText(FText::AsNumber(ModIDsWithErrors.Num()));
	}

	UModioModCategoryParamsUI* CategoryParams = Cast<UModioModCategoryParamsUI>(DataSource);
	const bool bDifferentSortField = [this, CategoryParams]()
	{
		if (CategoryParams)
		{
			return LastAppliedFilterParams.ManualSortField != CategoryParams->Underlying.ManualSortField;
		}
		return false;
	}();

	const FModioModCategoryParams DefaultCategoryParams = GetDefaultCategoryFilterParams(UISubsystem->IsUserAuthenticated());
	const int32 NumOfAppliedFilters = DefaultCategoryParams.GetNumOfDifferences(CategoryParams ? CategoryParams->Underlying : FModioModCategoryParams());

	if (FilterCounterTextBlock)
	{
		FilterCounterTextBlock->SetText(FText::FromString(FString::FromInt(NumOfAppliedFilters)));
	}

	if (ModList && ModList->Implements<UModioCommonModListViewInterface>() && !IsDesignTime()
		&& bDifferentSortField)
	{
		IModioCommonModListViewInterface::Execute_RequestFullClearSelection(ModList, true);
	}

	LastAppliedFilterParams = CategoryParams ? CategoryParams->Underlying : FModioModCategoryParams();

	if (NumOfInstalledModsTextBlock)
	{
		NumOfInstalledModsTextBlock->SetText(FText::AsNumber(NumOfInstalledMods));
	}

	if (ModList && ModList->Implements<UModioCommonModListViewInterface>() && !IsDesignTime())
	{
		IModioCommonModListViewInterface::Execute_SetModsFromModCollectionEntryArray(ModList, Entries, false);
		IModioCommonModListViewInterface::Execute_RequestFullClearSelection(ModList, false);
		if (bDifferentSortField && Entries.Num() > 0)
		{
			IModioCommonModListViewInterface::Execute_SetModSelectionByID(ModList, Entries[0].GetID());
		}
		ModList->RegenerateAllEntries();
	}

#if WITH_EDITOR
	if (!IsDesignTime())
#endif
	SetNoResultsVisibility(Entries.Num() <= 0);
}
void UModioCommonCollectionView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	UpdateMods();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (InstalledModsLabelTextBlock)
		{
			InstalledModsLabelTextBlock->SetText(UISettings->CollectionParams.InstalledModsLabel);
		}

		if (InstalledModsDescriptionTextBlock)
		{
			InstalledModsDescriptionTextBlock->SetText(UISettings->CollectionParams.InstalledModsDescription);
		}

		if (ErrorsTextBlock)
		{
			ErrorsTextBlock->SetText(UISettings->CollectionParams.ErrorsLabel);
		}

		if (FilterButton)
		{
			FilterButton->SetLabel(UISettings->CollectionParams.FilterButtonLabel);
		}

		if (FetchUpdateButton) 
		{
			FetchUpdateButton->SetLabel(UISettings->CollectionParams.DefaultFetchUpdateButtonLabel);
		}
	}

	if (UModioCommonCollectionViewStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (UModioCommonListView* ModListView = Cast<UModioCommonListView>(ModList))
		{
			ModListView->SetStyle(StyleCDO->ModListStyle);
		}
		else if (UModioCommonModTileView* ModTileView = Cast<UModioCommonModTileView>(ModList))
		{
			ModTileView->SetStyle(StyleCDO->ModListStyle);
		}
		
		if (NumOfDownloadingModsTextBlock)
		{
			NumOfDownloadingModsTextBlock->SetStyle(StyleCDO->NumOfDownloadingModsTextStyle);
		}

		if (InstalledModsLabelTextBlock)
		{
			InstalledModsLabelTextBlock->SetStyle(StyleCDO->InstalledModsLabelTextStyle);
		}

		if (InstalledModsDescriptionTextBlock)
		{
			InstalledModsDescriptionTextBlock->SetStyle(StyleCDO->InstalledModsDescriptionTextStyle);
		}

		if (NumOfInstalledModsTextBlock)
		{
			NumOfInstalledModsTextBlock->SetStyle(StyleCDO->NumOfInstalledModsTextStyle);
		}

		if (ErrorsTextBlock)
		{
			ErrorsTextBlock->SetStyle(StyleCDO->ErrorsTextStyle);
		}

		if (NumOfErrorsTextBlock)
		{
			NumOfErrorsTextBlock->SetStyle(StyleCDO->NumOfErrorsTextStyle);
		}

		if (FetchUpdateButton)
		{
			FetchUpdateButton->SetStyle(StyleCDO->FetchUpdateButtonStyle);
		}

		if (FilterButton)
		{
			FilterButton->SetStyle(StyleCDO->FilterButtonStyle);
		}

		if (FilterCounterTextBlock)
		{
			FilterCounterTextBlock->SetStyle(StyleCDO->FilterCounterTextStyle);
		}
	}
}

void UModioCommonCollectionView::NativePreConstruct()
{
	Super::NativePreConstruct();
#if WITH_EDITOR
	if (IsDesignTime())
	{
		SetNoResultsVisibility(bPreviewShowNoResults);
		SynchronizeProperties();
	}
#endif
}

UWidget* UModioCommonCollectionView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (ModList && ModList->GetNumItems() > 0 && ModList->Implements<UModioCommonModListViewInterface>())
	{
		if (UWidget* FocusTarget = IModioCommonModListViewInterface::Execute_GetDesiredListFocusTarget(ModList, true, true))
		{
			return FocusTarget;
		}
	}

	if (FilterButton)
	{
		return FilterButton;
	}

	if (FetchUpdateButton)
	{
		return FetchUpdateButton;
	}

	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonCollectionView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	IModioUIModManagementEventReceiver::Register<UModioCommonCollectionView>();
	IModioUISubscriptionsChangedReceiver::Register<UModioCommonCollectionView>();
	IModioUIUserChangedReceiver::Register<UModioCommonCollectionView>();

	SetNoResultsVisibility(false);

	UpdateInputBindings();

	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->OnUserChanged.AddWeakLambda(this, [this](TOptional<FModioUser> User) 
		{
			if (ModList)
			{
				ModList->ClearListItems();
			}

			// Category filter params are set based on whether the user has logged in or out.
			SetDefaultCategoryFilterParams(User.IsSet());

			UpdateMods();
			if (User.IsSet())
			{
				OnFetchUpdatesClicked();
			}
		});

		// When the collection view is initialized, the default category filter params are set to only filter the user mods.
		SetDefaultCategoryFilterParams(Subsystem->IsUserAuthenticated());
	}
}

void UModioCommonCollectionView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (TOptional<FModioModProgressInfo> CurrentProgress = Subsystem->QueryCurrentModUpdate())
		{
			//UpdateMods();
		}
	}
}

void UModioCommonCollectionView::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	ModIDsWithErrors.Empty();
	UpdateMods();

	if (FetchUpdateButton) 
	{
		FetchUpdateButton->SetVisibility(NewUser.IsSet() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonCollectionView::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);

	if (Event.Status)
	{
		ModIDsWithErrors.Add(Event.ID);
	}

	UpdateMods();
}

void UModioCommonCollectionView::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	UpdateMods();
}

void UModioCommonCollectionView::OnFetchUpdatesClicked_Implementation() 
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->FetchExternalUpdatesAsync(FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioCommonCollectionView::OnFetchExternalCompleted));

		if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
		{
			if (FetchUpdateButton)
			{
				FetchUpdateButton->SetIsEnabled(false);
				FetchUpdateButton->SetLabel(UISettings->CollectionParams.SearchingFetchUpdateButtonLabel);
			}
		}
	}
}

void UModioCommonCollectionView::OnFetchExternalCompleted_Implementation(FModioErrorCode ErrorCode) 
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->DisplayNotificationParams(UModioNotificationParamsLibrary::CreateNotificationParams(
			ErrorCode, LOCTEXT("FetchUpdatesSucceeded", "Updates fetched!"),
			LOCTEXT("FetchUpdatesSucceeded", "Updates fetched!"),
			LOCTEXT("FetchUpdatesFailed", "Could not fetch updates.")));
	}

	if (!ErrorCode)
	{
		UpdateMods();
	}

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (FetchUpdateButton)
		{
			// Delay re-enabling the button in one second to prevent spamming the server
			if (UWorld* World = GetWorld())
			{
				FTimerHandle TimerHandle;
				World->GetTimerManager().SetTimer(TimerHandle, [this]()
				{
					FetchUpdateButton->SetIsEnabled(true);
				}, 1, false);
			}
			FetchUpdateButton->SetLabel(UISettings->CollectionParams.DefaultFetchUpdateButtonLabel);
		}
	}
}

void UModioCommonCollectionView::ShowSearchView_Implementation()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioCommonModBrowser* ModBrowser = Cast<UModioCommonModBrowser>(Subsystem->GetModBrowserInstance()))
		{
			UModioModCategoryParamsUI* CategoryParams = Cast<UModioModCategoryParamsUI>(DataSource);
			FModioModCategoryParams FilterParams = CategoryParams ? CategoryParams->Underlying : FModioModCategoryParams();
			ModBrowser->ShowSearchView(EModioCommonSearchViewType::Collection, FilterParams, GetDefaultCategoryFilterParams(Subsystem->IsUserAuthenticated()));
		}
	}
}

void UModioCommonCollectionView::SetNoResultsVisibility_Implementation(bool bVisible)
{
	if (NoResultsContainer)
	{
		NoResultsContainer->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonCollectionView::SortAToZ(TArray<FModioModCollectionEntry>& ModListEntries)
{
	Algo::Sort(ModListEntries, [](const FModioModCollectionEntry& A, const FModioModCollectionEntry& B) {
		return FName(A.GetModProfile().ProfileName)
			.LexicalLess(FName(B.GetModProfile().ProfileName));
	});
}

void UModioCommonCollectionView::SortZToA(TArray<FModioModCollectionEntry>& ModListEntries)
{
	Algo::Sort(ModListEntries, [](const FModioModCollectionEntry& A, const FModioModCollectionEntry& B) {
		return FName(B.GetModProfile().ProfileName)
			.LexicalLess(FName(A.GetModProfile().ProfileName));
	});
}

void UModioCommonCollectionView::SortSizeOnDisk(TArray<FModioModCollectionEntry>& ModListEntries)
{
	Algo::Sort(ModListEntries, [](const FModioModCollectionEntry& A, const FModioModCollectionEntry& B) {
		return A.GetSizeOnDisk() > B.GetSizeOnDisk();
	});
}

void UModioCommonCollectionView::SortRecentlyUpdated(TArray<FModioModCollectionEntry>& ModListEntries)
{
	Algo::Sort(ModListEntries, [](const FModioModCollectionEntry& A, const FModioModCollectionEntry& B) {
		return A.GetModProfile().ProfileDateUpdated < B.GetModProfile().ProfileDateUpdated;
	});
}

void UModioCommonCollectionView::ApplySortingAndFiltering(TArray<FModioModCollectionEntry>& ModListEntries)
{
	UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!UISubsystem || !Subsystem)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to apply sorting and filtering to mod list in '%s': Modio Subsystem is not available"), *GetName());
		return;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& UserSubscriptions = Subsystem->QueryUserSubscriptions();

	FModioModCategoryParams FilterParams = [this]() {
		UModioModCategoryParamsUI* CategoryParams = Cast<UModioModCategoryParamsUI>(DataSource);
		if (CategoryParams)
		{
			return CategoryParams->Underlying;
		}
		return FModioModCategoryParams();
	}();

	FString SearchKeywords = FilterParams.SearchKeywords;
	SearchKeywords.TrimStartAndEndInline();

	const bool bFilterByEnabledState = UISubsystem->GetIsCollectionModDisableUIEnabled() && FilterParams.EnabledFilter != EModioEnabledFilterType::None;
	ModListEntries.RemoveAll([this, &FilterParams, bFilterByEnabledState, UISubsystem, &SearchKeywords, &UserSubscriptions](const FModioModCollectionEntry& Entry) {
		bool bRemove = false;
		if (!SearchKeywords.IsEmpty())
		{
			bRemove |= !Entry.GetModProfile().ProfileName.Contains(SearchKeywords);
		}

		if (!bRemove && FilterParams.InstalledField != EModioInstalledFilterType::None)
		{
			if (!bRemove && FilterParams.InstalledField == EModioInstalledFilterType::CurrentUser)
			{
				bRemove |= !UserSubscriptions.Contains(Entry.GetID());
			}
			else if (!bRemove && FilterParams.InstalledField == EModioInstalledFilterType::AnotherUser)
			{
				bRemove |= UserSubscriptions.Contains(Entry.GetID());
			}
		}

		if (!bRemove && FilterParams.QueuedField != EModioQueuedFilterType::None)
		{
			if (!bRemove && FilterParams.QueuedField == EModioQueuedFilterType::Queued)
			{
				bRemove |= Entry.GetModState() != EModioModState::UpdatePending
					&& Entry.GetModState() != EModioModState::InstallationPending
					&& Entry.GetModState() != EModioModState::UninstallPending;
			}
			else if (!bRemove && FilterParams.QueuedField == EModioQueuedFilterType::NotQueued)
			{
				bRemove |= Entry.GetModState() == EModioModState::UpdatePending
					|| Entry.GetModState() == EModioModState::InstallationPending
					|| Entry.GetModState() == EModioModState::UninstallPending;
			}
		}

		if (!bRemove && bFilterByEnabledState)
		{
			if (!bRemove && FilterParams.EnabledFilter == EModioEnabledFilterType::Enabled)
			{
				bRemove |= !UISubsystem->QueryIsModEnabled(Entry.GetID());
			}
			else if (!bRemove && FilterParams.EnabledFilter == EModioEnabledFilterType::Disabled)
			{
				bRemove |= UISubsystem->QueryIsModEnabled(Entry.GetID());
			}
		}

		if (!bRemove && (FilterParams.Tags.Num() > 0 || FilterParams.ExcludedTags.Num() > 0))
		{
			TSet<FModioModTag> EntryTags = TSet<FModioModTag>(Entry.GetModProfile().Tags);

			if (!bRemove)
			{
				for (const FString& Tag : FilterParams.Tags)
				{
					if (!EntryTags.Contains(FModioModTag(Tag)))
					{
						bRemove = true;
						break;
					}
				}
			}

			if (!bRemove)
			{
				for (const FString& Tag : FilterParams.ExcludedTags)
				{
					if (EntryTags.Contains(FModioModTag{Tag}))
					{
						bRemove = true;
						break;
					}
				}
			}
		}

		return bRemove;
	});

	switch (FilterParams.ManualSortField) {
		case EModioManualSortType::AToZ:
			SortAToZ(ModListEntries);
		break;
		case EModioManualSortType::ZToA:
			SortZToA(ModListEntries);
		break;
		case EModioManualSortType::SizeOnDisk:
			SortSizeOnDisk(ModListEntries);
		break;
		case EModioManualSortType::RecentlyUpdated:
			SortRecentlyUpdated(ModListEntries);
		break;
	}
}

void UModioCommonCollectionView::UpdateInputBindings_Implementation()
{
	ClearListeningInputActions();
	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (FetchUpdateButton)
		{
			ListenForInputAction(FetchUpdateButton, UISettings->CollectionParams.CheckForUpdatesInputAction, UISettings->CollectionParams.DefaultFetchUpdateButtonLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{ 
				OnFetchUpdatesClicked();
			}));
		}

		if (FilterButton)
		{
			ListenForInputAction(FilterButton, UISettings->CollectionParams.FilterInputAction, UISettings->CollectionParams.FilterButtonLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{ 
				ShowSearchView();
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

FModioModCategoryParams UModioCommonCollectionView::GetDefaultCategoryFilterParams_Implementation(bool bUserAuthenticated)
{
	FModioModCategoryParams DefaultFCategoryParamsStruct;
	DefaultFCategoryParamsStruct.InstalledField = bUserAuthenticated ? EModioInstalledFilterType::CurrentUser : EModioInstalledFilterType::None;
	return DefaultFCategoryParamsStruct;
}

void UModioCommonCollectionView::SetDefaultCategoryFilterParams_Implementation(bool bUserAuthenticated)
{
	UModioModCategoryParamsUI* DefaultUCategoryParamsObject = NewObject<UModioModCategoryParamsUI>();
	DefaultUCategoryParamsObject->Underlying = GetDefaultCategoryFilterParams(bUserAuthenticated);
	SetDataSource(DefaultUCategoryParamsObject);
}

#include "Loc/EndModioLocNamespace.h"
