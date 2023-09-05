/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Foundation/Components/List/ModioCommonModListView.h"
#include "Containers/Array.h"
#include "Core/ModioModInfoUI.h"

void UModioCommonModListView::SetModSelectionByID_Implementation(FModioModID ModID)
{
	IModioCommonModListViewInterface::SetModSelectionByID_Implementation(ModID);
	const TArray<UObject*> ModListItems = GetListItems();
	for (UObject* CurrentItem : ModListItems)
	{
		if (UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(CurrentItem))
		{
			if (IModioModInfoUIDetails::Execute_GetModID(ModInfo) == ModID)
			{
				SetSelectedItem(CurrentItem);
				RequestRefresh();
				return;
			}
		}
	}
}

void UModioCommonModListView::SetFocusOnceListIsPopulated_Implementation(bool bFocus)
{
	IModioCommonModListViewInterface::SetFocusOnceListIsPopulated_Implementation(bFocus);
	bFocusOnceListIsPopulatedRequested = bFocus;
}

void UModioCommonModListView::RequestFullClearSelection_Implementation()
{
	IModioCommonModListViewInterface::RequestFullClearSelection_Implementation();
	ClearSelection();
	for (UUserWidget* CurrentWidget : GetDisplayedEntryWidgets())
	{
		IUserListEntry::UpdateItemSelection(*CurrentWidget, false);
	}
	RequestRefresh();
}

void UModioCommonModListView::NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting)
{
	IModioCommonModListViewInterface::NativeSetListItems(InListItems, bAddToExisting);
	if (bAddToExisting)
	{
		for (UObject* Item : InListItems)
		{
			AddItem(Item);
		}
	}
	else
	{
		UObject* SelectedItem = GetSelectedItem<UObject>();
		const int32 SelectedIndex = SelectedItem ? GetIndexForItem(SelectedItem) : 0;
		SetListItems(InListItems);
		if (GetOwningPlayer() && (bFocusOnceListIsPopulatedRequested || HasAnyUserFocus() || HasFocusedDescendants() || SelectedItem))
		{
			SetSelectedIndex(SelectedIndex);
			SetFocus();
			bFocusOnceListIsPopulatedRequested = false;
		}
		RequestRefresh();
	}
}
