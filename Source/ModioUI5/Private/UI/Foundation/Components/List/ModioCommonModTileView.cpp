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
			AddItem(Item);
		}
	}
	else
	{
		SetListItems(InListItems);
		if (UWorld* World = Cast<UObject>(this)->GetWorld())
		{
			World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(Cast<UObject>(this), [this]() mutable {
				UObject* ModItem;
				TOptional<FModioModID> SelectedModID;
				if (Execute_GetSelectedModItem(Cast<UObject>(this), true, ModItem))
				{
					if (ModItem->Implements<UModioModInfoUIDetails>())
					{
						SelectedModID = IModioModInfoUIDetails::Execute_GetModID(ModItem);
					}
				}
				
				if (SelectedModID.IsSet() && GetOwningPlayer() && (bFocusOnceListIsPopulatedRequested || HasAnyUserFocus() || HasFocusedDescendants()))
				{
					UE_LOG(LogTemp, Error, TEXT("Setting mod selection to bFocusOnceListIsPopulatedRequested: %d, HasAnyUserFocus: %d, HasFocusedDescendants: %d"), bFocusOnceListIsPopulatedRequested, HasAnyUserFocus(), HasFocusedDescendants());
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