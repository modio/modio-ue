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
#include "ModioUI5.h"
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

void UModioCommonCollectionView::UpdateDownloadingMods_Implementation()
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& UserSubscriptions = Subsystem->QueryUserSubscriptions();
	
	// Rebuild the Pending Downloads list
	TArray<FModioModCollectionEntry> Entries;
	UserSubscriptions.GenerateValueArray(Entries);

	TArray<FModioModCollectionEntry> QueuedEntries = Entries;
	QueuedEntries.RemoveAll([](const FModioModCollectionEntry& Entry) {
		return Entry.GetModState() != EModioModState::UpdatePending
			   && Entry.GetModState() != EModioModState::InstallationPending;
	});
	
	Algo::Reverse(QueuedEntries);

	TArray<FModioModCollectionEntry> DownloadingEntries = Entries;
	DownloadingEntries.RemoveAll([](const FModioModCollectionEntry& Entry) {
		return Entry.GetModState() != EModioModState::Downloading
			   && Entry.GetModState() != EModioModState::Extracting;
	});
	ApplySortingAndFiltering(DownloadingEntries);
	ApplySortingAndFiltering(QueuedEntries);

	Entries.Empty();
	Entries.Append(MoveTemp(DownloadingEntries));
	Entries.Append(MoveTemp(QueuedEntries));

	if (NumOfDownloadingModsTextBlock)
	{
		NumOfDownloadingModsTextBlock->SetText(FText::AsNumber(Entries.Num()));
	}

	if (AreDownloadingSameMods(Entries))
	{
		return;
	}

	UModioModCategoryParamsUI* CategoryParams = Cast<UModioModCategoryParamsUI>(DataSource);
	LastAppliedDownloadingFilterParams = CategoryParams ? CategoryParams->Underlying : FModioModCategoryParams();
	if (DownloadingModList && DownloadingModList->Implements<UModioCommonModListViewInterface>() && !IsDesignTime())
	{
		IModioCommonModListViewInterface::Execute_SetModsFromModCollectionEntryArray(DownloadingModList, Entries, false);
	}
}

void UModioCommonCollectionView::UpdateInstalledMods_Implementation()
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& Installations = Subsystem->QuerySystemInstallations();

	TArray<FModioModCollectionEntry> Entries;
	Installations.GenerateValueArray(Entries);
	Entries.RemoveAll([](const FModioModCollectionEntry& Entry) {
		return Entry.GetModState() != EModioModState::Installed;
	});

	if (NumOfErrorsTextBlock)
	{
		NumOfErrorsTextBlock->SetText(FText::AsNumber(ModIDsWithErrors.Num()));
	}

	if (AreInstalledSameMods(Entries))
	{
		return;
	}

	UModioModCategoryParamsUI* CategoryParams = Cast<UModioModCategoryParamsUI>(DataSource);
	const bool bDifferentSortField = [this, CategoryParams]()
	{
		if (CategoryParams)
		{
			return LastAppliedInstalledFilterParams.ManualSortField != CategoryParams->Underlying.ManualSortField;
		}
		return false;
	}();
	if (InstalledModList && InstalledModList->Implements<UModioCommonModListViewInterface>() && !IsDesignTime()
		&& bDifferentSortField)
	{
		IModioCommonModListViewInterface::Execute_RequestFullClearSelection(InstalledModList, true);
	}

	LastAppliedInstalledFilterParams = CategoryParams ? CategoryParams->Underlying : FModioModCategoryParams();
	ApplySortingAndFiltering(Entries);

	if (NumOfInstalledModsTextBlock)
	{
		NumOfInstalledModsTextBlock->SetText(FText::AsNumber(Entries.Num()));
	}
	
	if (InstalledModList && InstalledModList->Implements<UModioCommonModListViewInterface>() && !IsDesignTime())
	{
		IModioCommonModListViewInterface::Execute_SetModsFromModCollectionEntryArray(InstalledModList, Entries, false);
		if (bDifferentSortField && Entries.Num() > 0)
		{
			IModioCommonModListViewInterface::Execute_SetModSelectionByID(InstalledModList, Entries[0].GetID());
		}
	}
}

bool UModioCommonCollectionView::AreDownloadingSameMods_Implementation(const TArray<FModioModCollectionEntry>& NewDownloadingMods) const
{
	if (!DownloadingModList || IsDesignTime())
	{
		return false;
	}

	UModioModCategoryParamsUI* CategoryParams = Cast<UModioModCategoryParamsUI>(DataSource);
	if (CategoryParams && LastAppliedDownloadingFilterParams != CategoryParams->Underlying)
	{
		return false;
	}

	const TArray<UObject*>& OldDownloadingMods = DownloadingModList->GetListItems();

	if (OldDownloadingMods.Num() != NewDownloadingMods.Num())
	{
		return false;
	}

	for (FModioModCollectionEntry NewDownloadingMod : NewDownloadingMods)
	{
		const bool bContains = OldDownloadingMods.ContainsByPredicate([&NewDownloadingMod](UObject* ListItem) {
			UModioModCollectionEntryUI* ModCollectionEntryUI = Cast<UModioModCollectionEntryUI>(ListItem);
			return ModCollectionEntryUI
			       && IModioModInfoUIDetails::Execute_GetModID(ModCollectionEntryUI) == NewDownloadingMod.GetID()
			       && ModCollectionEntryUI->Underlying.GetModState() == NewDownloadingMod.GetModState();
		});
		if (!bContains)
		{
			return false;
		}
	}

	return true;
}

bool UModioCommonCollectionView::AreInstalledSameMods_Implementation(const TArray<FModioModCollectionEntry>& NewInstalledMods) const
{
	if (!InstalledModList || IsDesignTime())
	{
		return false;
	}

	UModioModCategoryParamsUI* CategoryParams = Cast<UModioModCategoryParamsUI>(DataSource);
	if (CategoryParams && LastAppliedInstalledFilterParams != CategoryParams->Underlying)
	{
		return false;
	}

	const TArray<UObject*>& OldInstalledMods = InstalledModList->GetListItems();

	if (OldInstalledMods.Num() != NewInstalledMods.Num())
	{
		return false;
	}

	for (FModioModCollectionEntry NewDownloadingMod : NewInstalledMods)
	{
		const bool bContains = OldInstalledMods.ContainsByPredicate([&NewDownloadingMod](UObject* ListItem) {
			UModioModCollectionEntryUI* ModCollectionEntryUI = Cast<UModioModCollectionEntryUI>(ListItem);
			return ModCollectionEntryUI
				   && IModioModInfoUIDetails::Execute_GetModID(ModCollectionEntryUI) == NewDownloadingMod.GetID()
				   && ModCollectionEntryUI->Underlying.GetModState() == NewDownloadingMod.GetModState();
		});
		if (!bContains)
		{
			return false;
		}
	}

	return true;
}

void UModioCommonCollectionView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	UpdateDownloadingMods();
	UpdateInstalledMods();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (DownloadingModsLabelTextBlock)
		{
			DownloadingModsLabelTextBlock->SetText(UISettings->CollectionParams.DownloadingModsLabel);
		}

		if (DownloadingModsDescriptionTextBlock)
		{
			DownloadingModsDescriptionTextBlock->SetText(UISettings->CollectionParams.DownloadingModsDescription);
		}

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
	}

	if (UModioCommonCollectionViewStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (UModioCommonListView* ModListView = Cast<UModioCommonListView>(DownloadingModList))
		{
			ModListView->SetStyle(StyleCDO->ModListStyle);
		}
		else if (UModioCommonModTileView* ModTileView = Cast<UModioCommonModTileView>(DownloadingModList))
		{
			ModTileView->SetStyle(StyleCDO->ModListStyle);
		}

		if (UModioCommonListView* ModListView = Cast<UModioCommonListView>(InstalledModList))
		{
			ModListView->SetStyle(StyleCDO->ModListStyle);
		}
		else if (UModioCommonModTileView* ModTileView = Cast<UModioCommonModTileView>(InstalledModList))
		{
			ModTileView->SetStyle(StyleCDO->ModListStyle);
		}
		
		if (DownloadingModsLabelTextBlock)
		{
			DownloadingModsLabelTextBlock->SetStyle(StyleCDO->DownloadingModsLabelTextStyle);
		}

		if (DownloadingModsDescriptionTextBlock)
		{
			DownloadingModsDescriptionTextBlock->SetStyle(StyleCDO->DownloadingModsDescriptionTextStyle);
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
	}
}

UWidget* UModioCommonCollectionView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (LastFocusedModList == InstalledModList)
	{
		if (InstalledModList && InstalledModList->Implements<UModioCommonModListViewInterface>())
		{
			if (UWidget* FocusTarget = IModioCommonModListViewInterface::Execute_GetDesiredListFocusTarget(InstalledModList, true, true))
			{
				if (FocusTarget != InstalledModList)
				{
					return FocusTarget;
				}
			}
		}
	}

	if (DownloadingModList && DownloadingModList->Implements<UModioCommonModListViewInterface>())
	{
		if (UWidget* FocusTarget = IModioCommonModListViewInterface::Execute_GetDesiredListFocusTarget(DownloadingModList, true, true))
		{
			return FocusTarget;
		}
	}
	return DownloadingModList;
}

void UModioCommonCollectionView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	IModioUIModManagementEventReceiver::Register<UModioCommonCollectionView>();
	IModioUISubscriptionsChangedReceiver::Register<UModioCommonCollectionView>();
	IModioUIUserChangedReceiver::Register<UModioCommonCollectionView>();

	if (DownloadingModList)
	{
		DownloadingModList->OnItemSelectionChanged().AddWeakLambda(this, [this](UObject* Item) {
			if (Item && DownloadingModList && InstalledModList
				&& DownloadingModList->GetSelectedItem<UObject>() == Item)
			{
				LastFocusedModList = DownloadingModList;
				if (InstalledModList->Implements<UModioCommonModListViewInterface>())
				{
					IModioCommonModListViewInterface::Execute_RequestFullClearSelection(InstalledModList, false);
				}
			}
		});
	}

	if (InstalledModList)
	{
		InstalledModList->OnItemSelectionChanged().AddWeakLambda(this, [this](UObject* Item) {
			if (Item && DownloadingModList && InstalledModList
				&& InstalledModList->GetSelectedItem<UObject>() == Item)
			{
				LastFocusedModList = InstalledModList;
				if (DownloadingModList->Implements<UModioCommonModListViewInterface>())
				{
					IModioCommonModListViewInterface::Execute_RequestFullClearSelection(DownloadingModList, false);
				}
			}
		});
	}

	UpdateInputBindings();

	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->OnUserChanged.AddWeakLambda(this, [this](TOptional<FModioUser> User) 
		{
			if (DownloadingModList)
			{
				DownloadingModList->ClearListItems();
			}
			if (InstalledModList)
			{
				InstalledModList->ClearListItems();
			}
			UpdateDownloadingMods();
			UpdateInstalledMods();
			if (User.IsSet())
			{
				OnFetchUpdatesClicked();
			}
		});
	}
}

void UModioCommonCollectionView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (TOptional<FModioModProgressInfo> CurrentProgress = Subsystem->QueryCurrentModUpdate())
		{
			//UpdateDownloadingMods();
		}
	}
}

void UModioCommonCollectionView::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	UpdateDownloadingMods();
	UpdateInstalledMods();

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

	UpdateDownloadingMods();
	UpdateInstalledMods();
}

void UModioCommonCollectionView::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	UpdateDownloadingMods();
	UpdateInstalledMods();
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
		UpdateInstalledMods();
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
			ModBrowser->ShowSearchView(EModioCommonSearchViewType::Collection, CategoryParams ? CategoryParams->Underlying : FModioModCategoryParams());
		}
	}
}

void UModioCommonCollectionView::SortAToZ(TArray<FModioModCollectionEntry>& ModList)
{
	Algo::Sort(ModList, [](const FModioModCollectionEntry& A, const FModioModCollectionEntry& B) {
		return FName(A.GetModProfile().ProfileName)
			.LexicalLess(FName(B.GetModProfile().ProfileName));
	});
}

void UModioCommonCollectionView::SortZToA(TArray<FModioModCollectionEntry>& ModList)
{
	Algo::Sort(ModList, [](const FModioModCollectionEntry& A, const FModioModCollectionEntry& B) {
		return FName(B.GetModProfile().ProfileName)
			.LexicalLess(FName(A.GetModProfile().ProfileName));
	});
}

void UModioCommonCollectionView::SortSizeOnDisk(TArray<FModioModCollectionEntry>& ModList)
{
	Algo::Sort(ModList, [](const FModioModCollectionEntry& A, const FModioModCollectionEntry& B) {
		return A.GetSizeOnDisk() > B.GetSizeOnDisk();
	});
}

void UModioCommonCollectionView::SortRecentlyUpdated(TArray<FModioModCollectionEntry>& ModList)
{
	Algo::Sort(ModList, [](const FModioModCollectionEntry& A, const FModioModCollectionEntry& B) {
		return A.GetModProfile().ProfileDateUpdated < B.GetModProfile().ProfileDateUpdated;
	});
}

void UModioCommonCollectionView::ApplySortingAndFiltering(TArray<FModioModCollectionEntry>& ModList)
{
	UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!UISubsystem || !Subsystem)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to apply sorting and filtering to mod list in '%s': Modio Subsystem is not available"), *GetName());
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
	ModList.RemoveAll([this, &FilterParams, bFilterByEnabledState, UISubsystem, &SearchKeywords, &UserSubscriptions](const FModioModCollectionEntry& Entry) {
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
					if (!EntryTags.Contains(FModioModTag{Tag}))
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
			SortAToZ(ModList);
		break;
		case EModioManualSortType::ZToA:
			SortZToA(ModList);
		break;
		case EModioManualSortType::SizeOnDisk:
			SortSizeOnDisk(ModList);
		break;
		case EModioManualSortType::RecentlyUpdated:
			SortRecentlyUpdated(ModList);
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
		UE_LOG(ModioUI5, Error, TEXT("Unable to activate bottom buttons input bindings for '%s': Settings are invalid"), *GetName());
	}
	UnbindInputActions();
	BindInputActions();
}

#include "Loc/EndModioLocNamespace.h"