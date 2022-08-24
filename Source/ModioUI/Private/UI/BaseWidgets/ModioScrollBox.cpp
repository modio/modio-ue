// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Core/ModioUIHelpers.h"
#include "Widgets/Layout/SScrollBar.h"

TSharedRef<SWidget> UModioScrollBox::RebuildWidget()
{
	ScrollbarSlot = nullptr;
	if (ScrollBarAlignment != EModioScrollBoxBarAlignment::Default)
	{
		MyScrollbar = (SNew(SScrollBar)
						   .Style(&WidgetBarStyle)
						   .Orientation(Orientation)
						   .OnUserScrolled_UObject(this, &UModioScrollBox::SlateHandleUserScrolled));

		MyScrollbar->SetDragFocusCause(EFocusCause::Mouse);

		MyScrollBox = SNew(SScrollBox)
						  .Style(&WidgetStyle)
						  .ScrollBarStyle(&WidgetBarStyle)
						  .Orientation(Orientation)
						  .ConsumeMouseWheel(ConsumeMouseWheel)
						  .NavigationDestination(NavigationDestination)
						  .NavigationScrollPadding(NavigationScrollPadding)
						  .ScrollWhenFocusChanges(ScrollWhenFocusChanges)
						  .AnimateWheelScrolling(bAnimateWheelScrolling)
						  .WheelScrollMultiplier(WheelScrollMultiplier)
						  .OnUserScrolled(BIND_UOBJECT_DELEGATE(FOnUserScrolled, SlateHandleUserScrolled))
						  .ExternalScrollbar(MyScrollbar.ToSharedRef());

		for (UPanelSlot* PanelSlot : Slots)
		{
			if (UScrollBoxSlot* TypedSlot = Cast<UScrollBoxSlot>(PanelSlot))
			{
				TypedSlot->Parent = this;
				TypedSlot->BuildSlot(MyScrollBox.ToSharedRef());
			}
		}

		SAssignNew(MyGridForCustomScrollbar, SGridPanel);
		switch (ScrollBarAlignment)
		{
			case EModioScrollBoxBarAlignment::Left:
				MyGridForCustomScrollbar->SetRowFill(0, 1);
				// Auto-size the scrollbar
				MyGridForCustomScrollbar->SetColumnFill(0, 0);
				// Set the scrollbox to fill all space greedily
				MyGridForCustomScrollbar->SetColumnFill(1, 1);
				// Add the widgets
				MyGridForCustomScrollbar->AddSlot(0, 0)
					.Expose(ScrollbarSlot)
					.Padding(0, 0, AdditionalPadding, 0)
					.AttachWidget(MyScrollbar.ToSharedRef());
				MyGridForCustomScrollbar->AddSlot(1, 0).AttachWidget(MyScrollBox.ToSharedRef());
				break;
			case EModioScrollBoxBarAlignment::Right:
				MyGridForCustomScrollbar->SetRowFill(0, 1);
				// Set the scrollbox to fill all space greedily
				MyGridForCustomScrollbar->SetColumnFill(0, 1);
				// Auto-size the scrollbar
				MyGridForCustomScrollbar->SetColumnFill(1, 0);
				// Add the widgets
				MyGridForCustomScrollbar->AddSlot(1, 0)
					.Expose(ScrollbarSlot)
					.Padding(AdditionalPadding, 0, 0, 0)
					.AttachWidget(MyScrollbar.ToSharedRef());
				MyGridForCustomScrollbar->AddSlot(0, 0).AttachWidget(MyScrollBox.ToSharedRef());
				break;
			case EModioScrollBoxBarAlignment::Top:
				MyGridForCustomScrollbar->SetColumnFill(0, 1);
				// Auto-size the scrollbar
				MyGridForCustomScrollbar->SetRowFill(0, 0);
				// Set the scrollbox to fill all space greedily
				MyGridForCustomScrollbar->SetRowFill(1, 1);
				// Add the widgets
				MyGridForCustomScrollbar->AddSlot(0, 0)
					.Expose(ScrollbarSlot)
					.Padding(0, 0, 0, AdditionalPadding)
					.AttachWidget(MyScrollbar.ToSharedRef());
				MyGridForCustomScrollbar->AddSlot(0, 1).AttachWidget(MyScrollBox.ToSharedRef());
				break;
			case EModioScrollBoxBarAlignment::Bottom:
				MyGridForCustomScrollbar->SetColumnFill(0, 1);
				// Set the scrollbox to fill all space greedily
				MyGridForCustomScrollbar->SetRowFill(0, 1);
				// Auto-size the scrollbar
				MyGridForCustomScrollbar->SetRowFill(1, 0);
				// Add the widgets
				MyGridForCustomScrollbar->AddSlot(0, 1)
					.Expose(ScrollbarSlot)
					.Padding(0, AdditionalPadding, 0, 0)
					.AttachWidget(MyScrollbar.ToSharedRef());
				MyGridForCustomScrollbar->AddSlot(0, 0).AttachWidget(MyScrollBox.ToSharedRef());
				break;
		}

		return MyGridForCustomScrollbar.ToSharedRef();
	}
	else
	{
		return UScrollBox::RebuildWidget();
	}
}

void UModioScrollBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyScrollbar.Reset();
	MyGridForCustomScrollbar.Reset();
}

void UModioScrollBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (ScrollbarSlot != nullptr)
	{
		switch (ScrollBarAlignment)
		{
			case EModioScrollBoxBarAlignment::Left:
				ModioUIHelpers::SetPadding(*ScrollbarSlot, 0, 0, AdditionalPadding, 0);
				break;
			case EModioScrollBoxBarAlignment::Right:
				ModioUIHelpers::SetPadding(*ScrollbarSlot, AdditionalPadding, 0, 0, 0);
				break;
			case EModioScrollBoxBarAlignment::Top:
				ModioUIHelpers::SetPadding(*ScrollbarSlot, 0, 0, 0, AdditionalPadding);
				break;
			case EModioScrollBoxBarAlignment::Bottom:
				ModioUIHelpers::SetPadding(*ScrollbarSlot, 0, AdditionalPadding, 0, 0);
				break;
		}
	}
}
