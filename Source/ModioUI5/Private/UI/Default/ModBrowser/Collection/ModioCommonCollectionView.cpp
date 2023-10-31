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
#include "UI/Settings/Params/ModioCommonModCollectionParams.h"
#include "Components/ListView.h"
#include "Algo/Reverse.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "TimerManager.h"
#include "ModioUI5.h"
#include "UI/Foundation/Components/Tab/ModioCommonTabListWidgetBase.h"

// These are only for the internal identification of the tabs
const FName AllInstalledTabId = FName(TEXT("AllInstalled"));
const FName SystemModsTabId = FName(TEXT("SystemMods"));

void UModioCommonCollectionView::SetStyle(TSubclassOf<UModioCommonCollectionViewStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

void UModioCommonCollectionView::UpdateDownloadingMods_Implementation()
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return;
	}

	TMap<FModioModID, FModioModCollectionEntry> UserSubscriptions =
		ViewTabType == EModioCommonCollectionViewTabType::SystemMods
			? Subsystem->QuerySystemInstallations()
			: Subsystem->QueryUserSubscriptions();
	
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

	TMap<FModioModID, FModioModCollectionEntry> Installations =
		ViewTabType == EModioCommonCollectionViewTabType::SystemMods
			? Subsystem->QuerySystemInstallations()
			: Subsystem->QueryUserInstallations(true);

	TArray<FModioModCollectionEntry> Entries;
	Installations.GenerateValueArray(Entries);
	Entries.RemoveAll([](const FModioModCollectionEntry& Entry) {
		return Entry.GetModState() != EModioModState::Installed;
	});

	if (NumOfInstalledModsTextBlock)
	{
		NumOfInstalledModsTextBlock->SetText(FText::AsNumber(Entries.Num()));
	}

	if (NumOfErrorsTextBlock)
	{
		NumOfErrorsTextBlock->SetText(FText::AsNumber(ModIDsWithErrors.Num()));
	}

	if (AreInstalledSameMods(Entries))
	{
		return;
	}

	if (InstalledModList && InstalledModList->Implements<UModioCommonModListViewInterface>() && !IsDesignTime())
	{
		IModioCommonModListViewInterface::Execute_SetModsFromModCollectionEntryArray(InstalledModList, Entries, false);
	}
}

bool UModioCommonCollectionView::AreDownloadingSameMods_Implementation(const TArray<FModioModCollectionEntry>& NewDownloadingMods) const
{
	if (!DownloadingModList || IsDesignTime())
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

	if (const UModioCommonModCollectionParamsSettings* Settings = GetDefault<UModioCommonModCollectionParamsSettings>())
	{
		if (DownloadingModsLabelTextBlock)
		{
			DownloadingModsLabelTextBlock->SetText(Settings->DownloadingModsLabel);
		}

		if (DownloadingModsDescriptionTextBlock)
		{
			DownloadingModsDescriptionTextBlock->SetText(Settings->DownloadingModsDescription);
		}

		if (InstalledModsLabelTextBlock)
		{
			InstalledModsLabelTextBlock->SetText(Settings->InstalledModsLabel);
		}

		if (InstalledModsDescriptionTextBlock)
		{
			InstalledModsDescriptionTextBlock->SetText(Settings->InstalledModsDescription);
		}

		if (ErrorsTextBlock)
		{
			ErrorsTextBlock->SetText(Settings->ErrorsLabel);
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

		if (const UModioCommonModCollectionParamsSettings* Settings = GetDefault<UModioCommonModCollectionParamsSettings>())
		{
			if (TabList)
			{
				TabList->RemoveAllDynamicTabs();

				TabList->SetPreviousTabInputActionData(Settings->PreviousTabInputAction);
				TabList->SetNextTabInputActionData(Settings->NextTabInputAction);

				TabList->SetListeningForInput(false);
				TabList->SetListeningForInput(true);

				FModioCommonTabDescriptor AllInstalledTabInfo;
				{
					AllInstalledTabInfo.TabId = AllInstalledTabId;
					AllInstalledTabInfo.TabText = Settings->AllInstalledCategoryName;
					AllInstalledTabInfo.TabButtonType = StyleCDO->CategoryTabButtonClass;
					AllInstalledTabInfo.TabButtonStyle = StyleCDO->CategoryTabButtonStyle;
				}
				TabList->RegisterDynamicTab(AllInstalledTabInfo);

				FModioCommonTabDescriptor SystemModsTabInfo;
				{
					SystemModsTabInfo.TabId = SystemModsTabId;
					SystemModsTabInfo.TabText = Settings->SystemModsCategoryName;
					SystemModsTabInfo.TabButtonType = StyleCDO->CategoryTabButtonClass;
					SystemModsTabInfo.TabButtonStyle = StyleCDO->CategoryTabButtonStyle;
				}
				TabList->RegisterDynamicTab(SystemModsTabInfo);
			}
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
	}
}

UWidget* UModioCommonCollectionView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (!DownloadingModList || DownloadingModList->GetNumItems() <= 0)
	{
		if (InstalledModList && InstalledModList->Implements<UModioCommonModListViewInterface>())
		{
			if (InstalledModList->Implements<UModioCommonModListViewInterface>())
			{
				return IModioCommonModListViewInterface::Execute_GetDesiredListFocusTarget(InstalledModList);
			}
		}
		return InstalledModList;
	}
	if (DownloadingModList && DownloadingModList->Implements<UModioCommonModListViewInterface>())
	{
		if (DownloadingModList->Implements<UModioCommonModListViewInterface>())
		{
			return IModioCommonModListViewInterface::Execute_GetDesiredListFocusTarget(DownloadingModList);
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

	if (TabList)
	{
		TabList->OnTabSelectedFast.AddWeakLambda(this, [this](FName TabId) {
			if (!IsActivated())
			{
				return;
			}
			RefreshListByTabId(TabId);
		});
	}

	if (DownloadingModList)
	{
		DownloadingModList->OnItemSelectionChanged().AddWeakLambda(this, [this](UObject* Item) {
			if (Item && DownloadingModList && InstalledModList
				&& DownloadingModList->GetSelectedItem<UObject>() == Item)
			{
				InstalledModList->ClearSelection();
			}
		});
	}

	if (InstalledModList)
	{
		InstalledModList->OnItemSelectionChanged().AddWeakLambda(this, [this](UObject* Item) {
			if (Item && DownloadingModList && InstalledModList
				&& InstalledModList->GetSelectedItem<UObject>() == Item)
			{
				DownloadingModList->ClearSelection();
			}
		});
	}

	UpdateInputBindings();

	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->OnUserChanged.AddWeakLambda(this, [this](TOptional<FModioUser> User) 
		{
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
			UpdateDownloadingMods();
		}
	}
}

void UModioCommonCollectionView::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	UpdateDownloadingMods();
	UpdateInstalledMods();
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

		if (const UModioCommonModCollectionParamsSettings* Settings = GetDefault<UModioCommonModCollectionParamsSettings>())
		{
			if (FetchUpdateButton)
			{
				FetchUpdateButton->SetIsEnabled(false);
				FetchUpdateButton->SetLabel(Settings->SearchingFetchUpdateButtonLabel);
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

	if (const UModioCommonModCollectionParamsSettings* Settings = GetDefault<UModioCommonModCollectionParamsSettings>())
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
			FetchUpdateButton->SetLabel(Settings->DefaultFetchUpdateButtonLabel);
		}
	}
}

void UModioCommonCollectionView::RefreshListByTabId_Implementation(FName TabId)
{
#if WITH_EDITOR
	if (!IsDesignTime())
#endif
	{
		if (TabId.IsEqual(AllInstalledTabId))
		{
			ViewTabType = EModioCommonCollectionViewTabType::AllInstalled;
		}
		else if (TabId.IsEqual(SystemModsTabId))
		{
			ViewTabType = EModioCommonCollectionViewTabType::SystemMods;
		}
		else
		{
			UE_LOG(ModioUI5, Error, TEXT("Unable to refresh mod list in '%s' for tab '%s': Tab ID '%s' is not supported"), *GetName(), *TabId.ToString(), *TabId.ToString());
			return;
		}

		UpdateDownloadingMods();
		UpdateInstalledMods();
	}
}

void UModioCommonCollectionView::UpdateInputBindings()
{
	ClearListeningInputActions();
	if (const UModioCommonModCollectionParamsSettings* Settings = GetDefault<UModioCommonModCollectionParamsSettings>())
	{
		ListenForInputAction(FetchUpdateButton, Settings->CheckForUpdatesInputAction, Settings->DefaultFetchUpdateButtonLabel, [this]() 
		{ 
			OnFetchUpdatesClicked();
		});
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to activate bottom buttons input bindings for '%s': Settings are invalid"), *GetName());
	}
	UnbindInputActions();
	BindInputActions();
}

#include "Loc/EndModioLocNamespace.h"