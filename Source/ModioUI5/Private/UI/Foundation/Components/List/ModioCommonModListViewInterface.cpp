/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/List/ModioCommonModListViewInterface.h"

#include "Engine/Engine.h"
#include "ModioSubsystem.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "Core/ModioModInfoUI.h"
#include "Types/ModioModInfoList.h"
#include "Algo/Transform.h"
#include "Components/ListView.h"


void IModioCommonModListViewInterface::SetModSelectionByID_Implementation(FModioModID ModID)
{
	if (UListView* ListView = GetListView())
	{
		const TArray<UObject*> ModListItems = ListView->GetListItems();
		for (UObject* CurrentItem : ModListItems)
		{
			if (CurrentItem->Implements<UModioModInfoUIDetails>())
			{
				if (IModioModInfoUIDetails::Execute_GetModID(CurrentItem) == ModID)
				{
					ListView->SetSelectedItem(CurrentItem);
					if (UWidget* WidgetToFocus = Execute_GetDesiredListFocusTarget(Cast<UObject>(this)))
					{
						WidgetToFocus->SetFocus();
					}
					ListView->RequestRefresh();
					return;
				}
			}
		}
	}
}

void IModioCommonModListViewInterface::SetModsFromModInfoList_Implementation(const FModioModInfoList& InList, bool bAddToExisting)
{
	TArray<UObject*> WrappedModList;
	Algo::Transform(InList.GetRawList(), WrappedModList, [](const FModioModInfo& In) {
		UModioModInfoUI* WrappedMod = NewObject<UModioModInfoUI>();
		WrappedMod->Underlying = In;
		return WrappedMod;
	});
	NativeSetListItems(WrappedModList, bAddToExisting);
}

void IModioCommonModListViewInterface::SetModsFromModInfoArray_Implementation(const TArray<FModioModInfo>& InArray, bool bAddToExisting)
{
	TArray<UObject*> WrappedModList;
	Algo::Transform(InArray, WrappedModList, [](const FModioModInfo& In) {
		UModioModInfoUI* WrappedMod = NewObject<UModioModInfoUI>();
		WrappedMod->Underlying = In;
		return WrappedMod;
	});
	NativeSetListItems(WrappedModList, bAddToExisting);
}

void IModioCommonModListViewInterface::SetModsFromModCollectionEntryArray_Implementation(const TArray<FModioModCollectionEntry>& InArray, bool bAddToExisting)
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return;
	}
	
	TMap<FModioModID, FModioModCollectionEntry> UserSubscriptions = Subsystem->QueryUserSubscriptions();
		
	TArray<UObject*> CachedCollection;
	Algo::Transform(InArray, CachedCollection,
					[&UserSubscriptions](const FModioModCollectionEntry& In) {
						UModioModCollectionEntryUI* WrappedModCollectionEntry =
							NewObject<UModioModCollectionEntryUI>();
						WrappedModCollectionEntry->Underlying = In;
						WrappedModCollectionEntry->bCachedSubscriptionStatus = UserSubscriptions.Contains(In.GetID());
						return WrappedModCollectionEntry;
					});
	NativeSetListItems(CachedCollection, bAddToExisting);
}

void IModioCommonModListViewInterface::RequestFullClearSelection_Implementation()
{
	if (UListView* ListView = GetListView())
	{
		ListView->ClearSelection();
		for (UUserWidget* CurrentWidget : ListView->GetDisplayedEntryWidgets())
		{
			IUserListEntry::UpdateItemSelection(*CurrentWidget, false);
		}
		ListView->RequestRefresh();
	}
}

bool IModioCommonModListViewInterface::GetSelectedModItem_Implementation(bool bIncludePreviouslySelected, UObject*& OutModItem)
{
	if (UListView* ListView = GetListView())
	{
		if (UObject* SelectedItem = ListView->GetSelectedItem<UObject>())
		{
			OutModItem = SelectedItem;
			return true;
		}
		const TArray<UObject*> ModListItems = ListView->GetListItems();
		if (PreviouslySelectedModID.IsSet())
		{
			for (UObject* CurrentItem : ModListItems)
			{
				if (CurrentItem->Implements<UModioModInfoUIDetails>())
				{
					if (IModioModInfoUIDetails::Execute_GetModID(CurrentItem) == PreviouslySelectedModID.GetValue())
					{
						OutModItem = CurrentItem;
						return true;
					}
				}
			}
		}
		if (ModListItems.Num() > 0)
		{
			OutModItem = ModListItems[0];
			return true;
		}
	}
	return false;
}

bool IModioCommonModListViewInterface::GetEntryWidgetFromItem_Implementation(UObject* InItem, UWidget*& OutEntryWidget)
{
	if (UListView* ListView = GetListView())
	{
		UWidget* EntryWidget = ListView->GetEntryWidgetFromItem(InItem);
		if (EntryWidget)
		{
			OutEntryWidget = EntryWidget;
			return true;
		}
	}
	return false;
}

UWidget* IModioCommonModListViewInterface::GetDesiredListFocusTarget_Implementation()
{
	if (UListView* ListView = GetListView())
	{
		UObject* SelectedModItem;
		if (IModioCommonModListViewInterface::Execute_GetSelectedModItem(ListView, true, SelectedModItem))
		{
			if (UWidget* WidgetToFocus = ListView->GetEntryWidgetFromItem(SelectedModItem))
			{
				ListView->SetSelectedItem(SelectedModItem);
				return WidgetToFocus;
			}
		}
		return ListView;
	}
	return nullptr;
}
