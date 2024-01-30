/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Foundation/Components/List/ModioCommonModTileView.h"
#include "Containers/Array.h"
#include "Core/ModioModInfoUI.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UModioCommonModTileView::NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting)
{
	IModioCommonModListViewInterface::NativeSetListItems(InListItems, bAddToExisting);
	if (bAddToExisting)
	{
		for (UObject* Item : InListItems)
		{
			if (Item->Implements<UModioModInfoUIDetails>()) 
			{
				if(ListItems.ContainsByPredicate([Item](UObject* Obj) 
					{
						if (Obj->Implements<UModioModInfoUIDetails>())
						{
							return UModioModCollectionEntryUI::Execute_GetModID(Item) == UModioModCollectionEntryUI::Execute_GetModID(Obj);
						}
						return false;
					}))
				{
					continue;
				}
			}
			AddItem(Item);
		}
	}
	else
	{
		UObject* ModItem;
		TOptional<FModioModID> SelectedModID;
		if (Execute_GetSelectedModItem(Cast<UObject>(this), true, ModItem))
		{
			if (ModItem->Implements<UModioModInfoUIDetails>())
			{
				SelectedModID = IModioModInfoUIDetails::Execute_GetModID(ModItem);
			}
		}
		
		SetListItems(InListItems);
		if (UWorld* World = Cast<UObject>(this)->GetWorld())
		{
			World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(Cast<UObject>(this), [this, SelectedModID]() mutable {
				if (SelectedModID.IsSet() && GetOwningPlayer() && bFocusOnceListIsPopulatedRequested)
				{
					Execute_SetModSelectionByID(this, SelectedModID.GetValue());
					bFocusOnceListIsPopulatedRequested = false;
				}
			}));
		}
	}
}

void UModioCommonModTileView::SetFocusOnceListIsPopulated_Implementation(bool bFocus)
{
	IModioCommonModListViewInterface::SetFocusOnceListIsPopulated_Implementation(bFocus);
	bFocusOnceListIsPopulatedRequested = bFocus;
}

void UModioCommonModTileView::OnSelectionChangedInternal(NullableItemType FirstSelectedItem)
{
	if (FirstSelectedItem && FirstSelectedItem->Implements<UModioModInfoUIDetails>())
	{
		PreviouslySelectedModID = IModioModInfoUIDetails::Execute_GetModID(FirstSelectedItem);
	}
	Super::OnSelectionChangedInternal(FirstSelectedItem);
}