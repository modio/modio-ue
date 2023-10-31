/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioDrawerController.h"
#include "Input/Reply.h"
#include "Layout/Visibility.h"
#include "Misc/Attribute.h"
#include "UI/BaseWidgets/Slots/ModioDrawerControllerSlot.h"
#include "UI/Interfaces/IModioUIDownloadQueueWidget.h"
#include "ModioUISubsystem.h"
#include "UI/CommonComponents/ModioMenu.h"

UClass* UModioDrawerController::GetSlotClass() const
{
	return UModioDrawerControllerSlot::StaticClass();
}

void UModioDrawerController::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if (MyOverlay.IsValid())
	{
		CastChecked<UModioDrawerControllerSlot>(InSlot)->BuildSlot(MyOverlay.ToSharedRef());
	}
}

void UModioDrawerController::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (MyOverlay.IsValid())
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			MyOverlay->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

TSharedRef<SWidget> UModioDrawerController::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();
	Widget->SetOnMouseButtonUp(FPointerEventHandler::CreateUObject(this, &UModioDrawerController::OnMouseUp));

	IModioUIActionHandler::Register<UModioDrawerController>();
	return Widget;
}

FReply UModioDrawerController::OnMouseUp(const FGeometry&, const FPointerEvent&)
{
	for (UPanelSlot* CurrentSlot : Slots)
	{
		if (UModioDrawerControllerSlot* TargetSlot = Cast<UModioDrawerControllerSlot>(CurrentSlot))
		{
			TargetSlot->SetExpanded(false);
		}
	}
	UpdateVisibility();
	return FReply::Handled(); // TODO: @modio-ue4 will have to implement some kind of cache of the previously focused
							  // widget to re-focus maybe?
}

void UModioDrawerController::UpdateVisibility()
{
	for (UPanelSlot* CurrentSlot : Slots)
	{
		if (UModioDrawerControllerSlot* TargetSlot = Cast<UModioDrawerControllerSlot>(CurrentSlot))
		{
			if (TargetSlot->GetExpandedState())
			{
				SetVisibility(ESlateVisibility::Visible);
				return;
			}
		}
	}
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

bool UModioDrawerController::ToggleDrawerExpanded(int32 SlotIndex, bool bCloseOtherDrawers)
{
	bool bNewDrawerState = false;
	if (SlotIndex < Slots.Num())
	{
		if (UModioDrawerControllerSlot* TargetSlot = Cast<UModioDrawerControllerSlot>(Slots[SlotIndex]))
		{
			bNewDrawerState = !TargetSlot->GetExpandedState();
			SetDrawerExpanded(SlotIndex, bNewDrawerState, bCloseOtherDrawers);
		}
	}
	return bNewDrawerState;
}

bool UModioDrawerController::IsDrawerExpanded(int32 SlotIndex)
{
	if (SlotIndex < Slots.Num())
	{
		if (UModioDrawerControllerSlot* TargetSlot = Cast<UModioDrawerControllerSlot>(Slots[SlotIndex]))
		{
			return TargetSlot->GetExpandedState();
		}
	}

	return false;
}

void UModioDrawerController::SetDrawerExpanded(int32 SlotIndex, bool bExpandedState, bool bCloseOtherDrawers)
{
	if (SlotIndex < Slots.Num())
	{
		for (int32 CurrentSlotIndex = 0; CurrentSlotIndex < Slots.Num(); CurrentSlotIndex++)
		{
			if (UModioDrawerControllerSlot* TargetSlot = Cast<UModioDrawerControllerSlot>(Slots[CurrentSlotIndex]))
			{
				if (CurrentSlotIndex == SlotIndex)
				{
					TargetSlot->SetExpanded(bExpandedState);

					if (!bExpandedState)
					{
						CurrentAnimatingOutDrawer = SlotIndex;
						TargetSlot->OnDrawerAnimatedOut.AddUniqueDynamic(this,
																		 &UModioDrawerController::DrawerAnimatedOut);
					}
				}
				else
				{
					if (bCloseOtherDrawers)
					{
						TargetSlot->SetExpanded(false);
					}
				}
			}
		}
	}
	UpdateVisibility();

	// If we're closing the drawers, make sure to reset controller focus
	if (bExpandedState == false) 
	{
		OnDrawerClosed.Broadcast();
	}
}

bool UModioDrawerController::IsAnyDrawerExpanded() const
{
	for (UPanelSlot* CurrentSlot : Slots)
	{
		if (UModioDrawerControllerSlot* TargetSlot = Cast<UModioDrawerControllerSlot>(CurrentSlot))
		{
			if (TargetSlot->GetExpandedState())
			{
				return true;
			}
		}
	}
	return false;
}

void UModioDrawerController::CollapseAllDrawers()
{
	SetDrawerExpanded(0, false, true);
}

bool UModioDrawerController::SetFocusToActiveDrawer()
{
	for (UPanelSlot* CurrentSlot : Slots)
	{
		if (UModioDrawerControllerSlot* TargetSlot = Cast<UModioDrawerControllerSlot>(CurrentSlot))
		{
			if (TargetSlot->GetExpandedState() && TargetSlot->Content && TargetSlot->Content->HasFocusedDescendants()) 
			{
				return true;
			}
			if (TargetSlot->GetExpandedState() && TargetSlot->Content && !TargetSlot->Content->HasFocusedDescendants())
			{
				TargetSlot->Content->SetFocus();
				return true;
			}
		}
	}
	return false;
}

void UModioDrawerController::DrawerAnimatedOut()
{
	OnDrawerAnimatedOut.Broadcast(CurrentAnimatingOutDrawer);
}

void UModioDrawerController::NativeOnMenuAction(EMenuAction Action, UObject* OptionalData)
{
	switch (Action)
	{
		case EMenuAction::CloseDownloadDrawer:
			SetDrawerExpanded((int32) EModioMenuDrawer::EMMD_DownloadQueue, false, false);
			break;
		case EMenuAction::CloseSearchDrawer:
			SetDrawerExpanded((int32) EModioMenuDrawer::EMMD_RefineSearch, false, false);
			break;
		case EMenuAction::RefreshDownloadQueue:
			for (UWidget* Child : GetAllChildren())
			{
				if (Child->Implements<UModioUIDownloadQueueWidget>())
				{
					IModioUIDownloadQueueWidget::Execute_RefreshDownloadQueue(Child);
				}
			}
			break;
	}
}