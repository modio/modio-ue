/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/QuickAccess/ModioCommonQuickAccessTabView.h"

#include "ModioUI5.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"
#include "UI/Default/QuickAccess/ModioCommonQuickAccessTabViewStyle.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Border/ModioCommonBorder.h"
#include "UI/Foundation/Components/List/ModioCommonModListView.h"
#include "UI/Foundation/Components/List/ModioCommonModTileView.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Utilities/StorageSpaceTracker/ModioCommonStorageSpaceTrackerUserWidget.h"
#include "UI/Settings/Params/ModioCommonQuickAccessParams.h"
#include "Algo/Reverse.h"

void UModioCommonQuickAccessTabView::SetStyle(TSubclassOf<UModioCommonQuickAccessTabViewStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

UModioCommonQuickAccessTabView::UModioCommonQuickAccessTabView()
{
	bAutoBindInputAction = false;
}

void UModioCommonQuickAccessTabView::UpdateCurrentDownloadsMods_Implementation()
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return;
	}

	TMap<FModioModID, FModioModCollectionEntry> UserSubscriptions = Subsystem->QueryUserSubscriptions();
	
	// Rebuild the Pending Downloads list
	TArray<FModioModCollectionEntry> Entries;
	UserSubscriptions.GenerateValueArray(Entries);
	Entries.RemoveAll([](const FModioModCollectionEntry& Entry) {
		return Entry.GetModState() != EModioModState::Downloading
		&& Entry.GetModState() != EModioModState::Extracting;
	});

	if (CurrentDownloadsNumberTextBlock)
	{
		CurrentDownloadsNumberTextBlock->SetText(FText::Format(FText::FromString("({0})"), FText::AsNumber(Entries.Num())));
	}

	if (AreSameMods(CurrentDownloadsModList, Entries))
	{
		return;
	}

	if (CurrentDownloadsModList && CurrentDownloadsModList->Implements<UModioCommonModListViewInterface>() && !IsDesignTime())
	{
		IModioCommonModListViewInterface::Execute_SetModsFromModCollectionEntryArray(CurrentDownloadsModList, Entries, false);
	}
}

void UModioCommonQuickAccessTabView::UpdateInQueueMods_Implementation()
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return;
	}

	TMap<FModioModID, FModioModCollectionEntry> UserSubscriptions = Subsystem->QueryUserSubscriptions();
	
	// Rebuild the Pending Downloads list
	TArray<FModioModCollectionEntry> Entries;
	UserSubscriptions.GenerateValueArray(Entries);
	Entries.RemoveAll([](const FModioModCollectionEntry& Entry) {
		return Entry.GetModState() != EModioModState::UpdatePending
		&& Entry.GetModState() != EModioModState::InstallationPending;
	});

	Algo::Reverse(Entries);

	if (InQueueNumberTextBlock)
	{
		InQueueNumberTextBlock->SetText(FText::Format(FText::FromString("({0})"), FText::AsNumber(Entries.Num())));
	}

	if (AreSameMods(InQueueModList, Entries))
	{
		return;
	}

	if (InQueueModList && InQueueModList->Implements<UModioCommonModListViewInterface>() && !IsDesignTime())
	{
		IModioCommonModListViewInterface::Execute_SetModsFromModCollectionEntryArray(InQueueModList, Entries, false);
	}
}

void UModioCommonQuickAccessTabView::UpdateCompletedNumber_Implementation()
{
	if (CompletedNumberTextBlock)
	{
		CompletedNumberTextBlock->SetText(FText::Format(FText::FromString("({0})"), FText::AsNumber(CachedNumOfCurrentlyInstalledMods)));
	}
}

void UModioCommonQuickAccessTabView::ContainsMod_Implementation(UListView* ModsView, const FModioModCollectionEntry& ModEntry, bool& bContains, bool& bDifferentState) const
{
	bContains = false;
	bDifferentState = false;
	
	if (!ModsView || IsDesignTime())
	{
		return;
	}

	const TArray<UObject*>& Mods = ModsView->GetListItems();

	for (UObject* ListItem : Mods)
	{
		UModioModCollectionEntryUI* ModCollectionEntryUI = Cast<UModioModCollectionEntryUI>(ListItem);
		if (ModCollectionEntryUI && IModioModInfoUIDetails::Execute_GetModID(ModCollectionEntryUI) == ModEntry.GetID())
		{
			bContains = true;
			bDifferentState = ModCollectionEntryUI->Underlying.GetModState() != ModEntry.GetModState();
			return;
		}
	}
}

bool UModioCommonQuickAccessTabView::AreSameMods_Implementation(UListView* ModsView, const TArray<FModioModCollectionEntry>& NewDownloadingMods) const
{
	if (!ModsView || IsDesignTime())
	{
		return false;
	}

	if (ModsView->GetNumItems() != NewDownloadingMods.Num())
	{
		return false;
	}

	for (FModioModCollectionEntry NewDownloadingMod : NewDownloadingMods)
	{
		bool bContains, bDifferentState;
		ContainsMod(ModsView, NewDownloadingMod, bContains, bDifferentState);
		if (!bContains || bDifferentState)
		{
			return false;
		}
	}

	return true;
}

void UModioCommonQuickAccessTabView::HandleOnCloseButtonClicked_Implementation()
{
	DeactivateWidget();
}

void UModioCommonQuickAccessTabView::HandleOnMyProfileButtonClicked_Implementation()
{
	UE_LOG(ModioUI5, Log, TEXT("UModioCommonQuickAccessTabView::HandleOnMyProfileButtonClicked_Implementation - Not implemented"));
}

void UModioCommonQuickAccessTabView::HandleOnAuthButtonClicked_Implementation()
{
	if (!IsUserAuthenticated())
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (Subsystem->ModBrowserInstance->Implements<UModioModBrowserInterface>())
			{
				IModioModBrowserInterface::Execute_ShowUserAuth(Subsystem->ModBrowserInstance);
				HandleOnCloseButtonClicked();
			}
		}
	}
	else
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			IModioModBrowserInterface::Execute_LogOut(Subsystem->ModBrowserInstance);
		}
	}
}

void UModioCommonQuickAccessTabView::HandleOnCollectionButtonClicked_Implementation()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioCommonModBrowser* ModBrowser = Cast<UModioCommonModBrowser>(Subsystem->ModBrowserInstance))
		{
			ModBrowser->ShowCollectionView();
			DeactivateWidget();
		}
	}
}

UWidget* UModioCommonQuickAccessTabView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (CurrentDownloadsModList && CurrentDownloadsModList->GetNumItems() > 0)
	{
		return CurrentDownloadsModList;
	}
	if (InQueueModList && InQueueModList->GetNumItems() > 0)
	{
		return InQueueModList;
	}
	if (CompletedModList && CompletedModList->GetNumItems() > 0)
	{
		return CompletedModList;
	}
	return CloseButton;
}

void UModioCommonQuickAccessTabView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUIModManagementEventReceiver::Register<UModioCommonQuickAccessTabView>();
	IModioUISubscriptionsChangedReceiver::Register<UModioCommonQuickAccessTabView>();
	IModioUIUserChangedReceiver::Register<UModioCommonQuickAccessTabView>();

	if (CurrentDownloadsModList)
	{
		CurrentDownloadsModList->OnItemSelectionChanged().AddWeakLambda(this, [this](UObject* Item) {
			if (Item && CurrentDownloadsModList && InQueueModList && CompletedModList)
			{
				IModioCommonModListViewInterface::Execute_RequestFullClearSelection(InQueueModList);
				IModioCommonModListViewInterface::Execute_RequestFullClearSelection(CompletedModList);
			}
			UpdateInputActions();
		});
	}

	if (InQueueModList)
	{
		InQueueModList->OnItemSelectionChanged().AddWeakLambda(this, [this](UObject* Item) {
			if (Item && CurrentDownloadsModList && InQueueModList && CompletedModList
				&& InQueueModList->GetSelectedItem<UObject>() == Item)
			{
				IModioCommonModListViewInterface::Execute_RequestFullClearSelection(CurrentDownloadsModList);
				IModioCommonModListViewInterface::Execute_RequestFullClearSelection(CompletedModList);
			}
			UpdateInputActions();
		});
	}

	if (CompletedModList)
	{
		CompletedModList->OnItemSelectionChanged().AddWeakLambda(this, [this](UObject* Item) {
			if (Item && CurrentDownloadsModList && InQueueModList && CompletedModList
				&& CompletedModList->GetSelectedItem<UObject>() == Item)
			{
				IModioCommonModListViewInterface::Execute_RequestFullClearSelection(InQueueModList);
				IModioCommonModListViewInterface::Execute_RequestFullClearSelection(CurrentDownloadsModList);
			}
			UpdateInputActions();
		});
	}

	if (OverlayBackgroundBorder)
	{
		OverlayBackgroundBorder->OnModioCommonBorderClicked.AddWeakLambda(this, [this]() {
			HandleOnCloseButtonClicked();
		});
	}

	if (CloseButton)
	{
		CloseButton->OnFocusReceived().AddWeakLambda(this, [this]() {
			ClearModsViewSelection();
			UpdateInputActions();
		});
	}

	if (MyProfileButton)
	{
		const UModioCommonQuickAccessParamsSettings* Settings = GetDefault<UModioCommonQuickAccessParamsSettings>();
		if (Settings) 
		{
			MyProfileButton->SetVisibility(Settings->ShowProfileButton ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			
			if (Settings->ShowProfileButton) 
			{
				MyProfileButton->OnFocusReceived().AddWeakLambda(this, [this]() {
					ClearModsViewSelection();
					UpdateInputActions();
				});
			}
		}
	}

	if (AuthButton)
	{
		AuthButton->OnFocusReceived().AddWeakLambda(this, [this]() {
			ClearModsViewSelection();
			UpdateInputActions();
		});
	}

	if (MyCollectionButton)
	{
		MyCollectionButton->OnFocusReceived().AddWeakLambda(this, [this]() {
			if (GetOwningPlayer() && CompletedModList && CompletedModList->GetNumItems() > 0)
			{
				CompletedModList->SetFocus();
			}
			else
			{
				ClearModsViewSelection();
			}
			UpdateInputActions();
		});
	}
}

void UModioCommonQuickAccessTabView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();

	if (!Subsystem)
	{
		return;
	}

	if (TOptional<FModioModProgressInfo> CurrentProgress = Subsystem->QueryCurrentModUpdate())
	{
		UpdateCurrentDownloadsMods();
		UpdateInQueueMods();
	}
}

void UModioCommonQuickAccessTabView::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);
	UpdateCurrentDownloadsMods();
	UpdateInQueueMods();

	if (!Event.Status)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			TMap<FModioModID, FModioModCollectionEntry> UserInstallation = Subsystem->QuerySystemInstallations();
			TOptional<FModioModProgressInfo> ProgressInfo = Subsystem->QueryCurrentModUpdate();
		
			if (FModioModCollectionEntry* EntryPtr = UserInstallation.Find(Event.ID))
			{
				if (Event.Event == EModioModManagementEventType::Installed)
				{
					bool bContains, bUnusedDifferentState;
					ContainsMod(CompletedModList, *EntryPtr, bContains, bUnusedDifferentState);

					if (!bContains)
					{
						if (CompletedModList && CompletedModList->Implements<UModioCommonModListViewInterface>() && !IsDesignTime())
						{
							IModioCommonModListViewInterface::Execute_SetModsFromModCollectionEntryArray(CompletedModList, {*EntryPtr}, true);
						}
						++CachedNumOfCurrentlyInstalledMods;
					}
				}
				else if (Event.Event == EModioModManagementEventType::Uninstalled)
				{
					if (CompletedModList)
					{
						bool bContains, bUnusedDifferentState;
						ContainsMod(CompletedModList, *EntryPtr, bContains, bUnusedDifferentState);
						if (bContains)
						{
							--CachedNumOfCurrentlyInstalledMods;
						}
					}
				}

				UpdateCompletedNumber();
			}
		}
	}
}

void UModioCommonQuickAccessTabView::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	UpdateCurrentDownloadsMods();
	UpdateInQueueMods();
}

void UModioCommonQuickAccessTabView::ClearModsViewSelection_Implementation()
{
	if (CurrentDownloadsModList && CurrentDownloadsModList->Implements<UModioCommonModListViewInterface>())
	{
		IModioCommonModListViewInterface::Execute_RequestFullClearSelection(CurrentDownloadsModList);
	}

	if (InQueueModList && InQueueModList->Implements<UModioCommonModListViewInterface>())
	{
		IModioCommonModListViewInterface::Execute_RequestFullClearSelection(InQueueModList);
	}

	if (CompletedModList && CompletedModList->Implements<UModioCommonModListViewInterface>())
	{
		IModioCommonModListViewInterface::Execute_RequestFullClearSelection(CompletedModList);
	}
}

void UModioCommonQuickAccessTabView::UpdateInputActions_Implementation()
{
	ClearListeningInputActions();

	if (const UModioCommonQuickAccessParamsSettings* Settings = GetDefault<UModioCommonQuickAccessParamsSettings>())
	{
		if (CloseButton)
		{
			ListenForInputAction(CloseButton, Settings->CloseInputAction, Settings->CloseButtonLabel, [this]() {
				HandleOnCloseButtonClicked();
			});
		}

		if (MyProfileButton)
		{
			ListenForInputAction(MyProfileButton, Settings->MyProfileInputAction, Settings->MyProfileButtonLabel, [this]() {
				HandleOnMyProfileButtonClicked();
			});
		}

		if (AuthButton)
		{
			ListenForInputAction(AuthButton, Settings->AuthInputAction, IsUserAuthenticated() ? Settings->LogOutButtonLabel : Settings->LogInButtonLabel, [this]() {
				HandleOnAuthButtonClicked();
			});
		}

		if (MyCollectionButton)
		{
			ListenForInputAction(MyCollectionButton, Settings->MyCollectionInputAction, Settings->MyCollectionButtonLabel, [this]() {
				HandleOnCollectionButtonClicked();
			});
		}
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to update input bindings for '%s': Settings are invalid"), *GetName());
	}

	UnbindInputActions();
	BindInputActions();
}

void UModioCommonQuickAccessTabView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	UpdateCurrentDownloadsMods();
	UpdateInQueueMods();
	UpdateCompletedNumber();

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (UserNameTextBlock)
		{
			TOptional<FModioUser> User = Subsystem->QueryUserProfile();
			if (User.IsSet())
			{
				UserNameTextBlock->SetText(FText::FromString(User.GetValue().Username));
			}
			else
			{
				// TODO
				UserNameTextBlock->SetText(FText::FromString("Guest"));
			}
		}

		if (InstalledModsNumberTextBlock)
		{
			InstalledModsNumberTextBlock->SetText(FText::AsNumber(Subsystem->QueryUserInstallations(false).Num()));
		}
	}

	if (const UModioCommonQuickAccessParamsSettings* Settings = GetDefault<UModioCommonQuickAccessParamsSettings>())
	{
		if (CloseButton)
		{
			CloseButton->SetLabel(Settings->CloseButtonLabel);
		}

		if (MyProfileButton)
		{
			MyProfileButton->SetLabel(Settings->MyProfileButtonLabel);
		}

		if (AuthButton)
		{
			AuthButton->SetLabel(IsUserAuthenticated() ? Settings->LogOutButtonLabel : Settings->LogInButtonLabel);
		}

		if (MyCollectionButton)
		{
			MyCollectionButton->SetLabel(Settings->MyCollectionButtonLabel);
		}

		if (CurrentDownloadsLabelTextBlock)
		{
			CurrentDownloadsLabelTextBlock->SetText(Settings->CurrentDownloadsLabel);
		}

		if (InQueueLabelTextBlock)
		{
			InQueueLabelTextBlock->SetText(Settings->InQueueLabel);
		}

		if (CompletedLabelTextBlock)
		{
			CompletedLabelTextBlock->SetText(Settings->CompletedLabel);
		}

		if (InstalledModsLabelTextBlock)
		{
			InstalledModsLabelTextBlock->SetText(Settings->InstalledModsLabel);
		}
	}
	
	if (UModioCommonQuickAccessTabViewStyle* StyleCDO = Cast<UModioCommonQuickAccessTabViewStyle>(ModioStyle.GetDefaultObject()))
	{
		if (UModioCommonListView* ModListView = Cast<UModioCommonListView>(CurrentDownloadsModList))
		{
			ModListView->SetStyle(StyleCDO->ModListStyle);
		}
		else if (UModioCommonModTileView* ModTileView = Cast<UModioCommonModTileView>(CurrentDownloadsModList))
		{
			ModTileView->SetStyle(StyleCDO->ModListStyle);
		}

		if (UModioCommonListView* ModListView = Cast<UModioCommonListView>(InQueueModList))
		{
			ModListView->SetStyle(StyleCDO->ModListStyle);
		}
		else if (UModioCommonModTileView* ModTileView = Cast<UModioCommonModTileView>(InQueueModList))
		{
			ModTileView->SetStyle(StyleCDO->ModListStyle);
		}

		if (UModioCommonListView* ModListView = Cast<UModioCommonListView>(CompletedModList))
		{
			ModListView->SetStyle(StyleCDO->ModListStyle);
		}
		else if (UModioCommonModTileView* ModTileView = Cast<UModioCommonModTileView>(CompletedModList))
		{
			ModTileView->SetStyle(StyleCDO->ModListStyle);
		}
		
		if (InternalBackgroundBorder)
		{
			InternalBackgroundBorder->SetStyle(StyleCDO->InternalBackgroundBorderStyle);
		}

		if (OverlayBackgroundBorder)
		{
			OverlayBackgroundBorder->SetStyle(StyleCDO->OverlayBackgroundBorderStyle);
		}

		if (UserNameTextBlock)
		{
			UserNameTextBlock->SetStyle(StyleCDO->UserNameTextStyle);
		}

		if (StorageSpaceTrackerUserWidget)
		{
			StorageSpaceTrackerUserWidget->SetStyle(StyleCDO->StorageSpaceTrackerStyle);
		}

		if (InstalledModsLabelTextBlock)
		{
			InstalledModsLabelTextBlock->SetStyle(StyleCDO->InstalledModsLabelTextStyle);
		}

		if (InstalledModsNumberTextBlock)
		{
			InstalledModsNumberTextBlock->SetStyle(StyleCDO->InstalledModsNumberTextStyle);
		}

		if (CurrentDownloadsLabelTextBlock)
		{
			CurrentDownloadsLabelTextBlock->SetStyle(StyleCDO->CurrentDownloadsLabelTextStyle);
		}

		if (CurrentDownloadsNumberTextBlock)
		{
			CurrentDownloadsNumberTextBlock->SetStyle(StyleCDO->CurrentDownloadsNumberTextStyle);
		}

		if (InQueueLabelTextBlock)
		{
			InQueueLabelTextBlock->SetStyle(StyleCDO->InQueueLabelTextStyle);
		}

		if (InQueueNumberTextBlock)
		{
			InQueueNumberTextBlock->SetStyle(StyleCDO->InQueueNumberTextStyle);
		}

		if (CompletedLabelTextBlock)
		{
			CompletedLabelTextBlock->SetStyle(StyleCDO->CompletedLabelTextStyle);
		}

		if (CompletedNumberTextBlock)
		{
			CompletedNumberTextBlock->SetStyle(StyleCDO->CompletedNumberTextStyle);
		}

		if (CloseButton)
		{
			CloseButton->SetStyle(StyleCDO->CloseButtonStyle);
		}

		if (MyProfileButton)
		{
			MyProfileButton->SetStyle(StyleCDO->MyProfileButtonStyle);
		}

		if (AuthButton)
		{
			AuthButton->SetStyle(StyleCDO->AuthButtonStyle);
		}

		if (MyCollectionButton)
		{
			MyCollectionButton->SetStyle(StyleCDO->MyCollectionButtonStyle);
		}
	}
}

void UModioCommonQuickAccessTabView::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	Super::NativeUserChanged(NewUser);
	CachedNumOfCurrentlyInstalledMods = 0;

	UpdateCurrentDownloadsMods();
	UpdateInQueueMods();

	if (CurrentDownloadsModList)
	{
		CurrentDownloadsModList->ClearListItems();
	}

	SynchronizeProperties();
}
