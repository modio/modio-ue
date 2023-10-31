/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/ScrollBox/ModioCommonScrollBox.h"

#include "UI/Foundation/Components/ScrollBar/ModioCommonScrollBarStyle.h"
#include "UI/Foundation/Components/ScrollBox/ModioCommonScrollBoxStyle.h"
#include "Misc/EngineVersionComparison.h"

void UModioCommonScrollBox::SetStyle(TSubclassOf<UModioCommonScrollBoxStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

void UModioCommonScrollBox::SynchronizeProperties()
{
	if (UModioCommonScrollBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
#if UE_VERSION_NEWER_THAN(5, 2, 0)
		SetWidgetStyle(StyleCDO->Style);
		if (UModioCommonScrollBarStyle* ScrollBarStyleCDO = StyleCDO->ScrollBarStyle.GetDefaultObject())
		{
			SetWidgetBarStyle(ScrollBarStyleCDO->Style);
			SetOrientation(ScrollBarStyleCDO->Orientation);
			SetScrollbarThickness(ScrollBarStyleCDO->Thickness);
			SetScrollbarPadding(ScrollBarStyleCDO->Padding);
			SetAlwaysShowScrollbar(ScrollBarStyleCDO->bAlwaysShowScrollbar);
			SetAlwaysShowScrollbarTrack(ScrollBarStyleCDO->bAlwaysShowScrollbarTrack);
		}
		SetScrollBarVisibility(StyleCDO->ScrollBarVisibility);
		SetConsumeMouseWheel(StyleCDO->ConsumeMouseWheel);
		SetAllowOverscroll(StyleCDO->AllowOverscroll);
		if (!MyScrollBox.IsValid())
		{
			InitBackPadScrolling(StyleCDO->BackPadScrolling);
			InitFrontPadScrolling(StyleCDO->FrontPadScrolling);
		}
		SetAnimateWheelScrolling(StyleCDO->bAnimateWheelScrolling);
		SetNavigationDestination(StyleCDO->NavigationDestination);
		if (!MyScrollBox.IsValid())
		{
			InitNavigationScrollPadding(StyleCDO->NavigationScrollPadding);
		}
		SetAllowRightClickDragScrolling(StyleCDO->bAllowRightClickDragScrolling);
		SetWheelScrollMultiplier(StyleCDO->WheelScrollMultiplier);
#else
		WidgetStyle = StyleCDO->Style;
		if (UModioCommonScrollBarStyle* ScrollBarStyleCDO = StyleCDO->ScrollBarStyle.GetDefaultObject())
		{
			WidgetBarStyle = ScrollBarStyleCDO->Style;
			Orientation = ScrollBarStyleCDO->Orientation;
			ScrollbarThickness = ScrollBarStyleCDO->Thickness;
			ScrollbarPadding = ScrollBarStyleCDO->Padding;
			AlwaysShowScrollbar = ScrollBarStyleCDO->bAlwaysShowScrollbar;
			AlwaysShowScrollbarTrack = ScrollBarStyleCDO->bAlwaysShowScrollbarTrack;
		}
		ScrollBarVisibility = StyleCDO->ScrollBarVisibility;
		ConsumeMouseWheel = StyleCDO->ConsumeMouseWheel;
		AllowOverscroll = StyleCDO->AllowOverscroll;
		BackPadScrolling = StyleCDO->BackPadScrolling;
		FrontPadScrolling = StyleCDO->FrontPadScrolling;
		bAnimateWheelScrolling = StyleCDO->bAnimateWheelScrolling;
		NavigationDestination = StyleCDO->NavigationDestination;
		NavigationScrollPadding = StyleCDO->NavigationScrollPadding;
		ScrollWhenFocusChanges = StyleCDO->ScrollWhenFocusChanges;
		bAllowRightClickDragScrolling = StyleCDO->bAllowRightClickDragScrolling;
		WheelScrollMultiplier = StyleCDO->WheelScrollMultiplier;
#endif
	}
	Super::SynchronizeProperties();
	UpdateNavigationData(GetScrollOffset());
}

TSharedRef<SWidget> UModioCommonScrollBox::RebuildWidget()
{
	UpdateNavigationData(GetScrollOffset());
	OnUserScrolled.RemoveDynamic(this, &UModioCommonScrollBox::OnUserScrolledHandle);
	OnUserScrolled.AddDynamic(this, &UModioCommonScrollBox::OnUserScrolledHandle);
	return Super::RebuildWidget();
}

void UModioCommonScrollBox::OnUserScrolledHandle_Implementation(float CurrentOffset)
{
	UpdateNavigationData(CurrentOffset);
}

void UModioCommonScrollBox::UpdateNavigationData_Implementation(int32 PendingScrollOffset)
{
	if (!IsValid(Navigation))
	{
		return;
	}

	if (IsDesignTime() || !IsValid(UserDefinedNavigation))
	{
		UserDefinedNavigation = NewObject<UWidgetNavigation>(this);
		UserDefinedNavigation->Up = Navigation->Up;
		UserDefinedNavigation->Down = Navigation->Down;
		UserDefinedNavigation->Left = Navigation->Left;
		UserDefinedNavigation->Right = Navigation->Right;
	}

	if (UModioCommonScrollBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		FCustomWidgetNavigationDelegate NavigationDelegate;
		NavigationDelegate.BindUFunction(this, "HandleCustomBoundaryNavigation");

		if (Orientation == EOrientation::Orient_Vertical)
		{
			SetNavigationRuleCustomBoundary(EUINavigation::Down, NavigationDelegate);
			SetNavigationRuleCustomBoundary(EUINavigation::Up, NavigationDelegate);
		}
		else if (Orientation == EOrientation::Orient_Horizontal)
		{
			SetNavigationRuleCustomBoundary(EUINavigation::Right, NavigationDelegate);
			SetNavigationRuleCustomBoundary(EUINavigation::Left, NavigationDelegate);
		}

		if (!StyleCDO->bScrollByNavigationInput || IsDesignTime())
		{
			Navigation = UserDefinedNavigation;
			BuildNavigation();
		}
		else if (PendingScrollOffset != INDEX_NONE && (PendingScrollOffset <= 0 || PendingScrollOffset >= GetScrollOffsetOfEnd()))
		{
			if (PendingScrollOffset <= 0)
			{
				Navigation->Up = UserDefinedNavigation->Up;
				Navigation->Left = UserDefinedNavigation->Left;
			}
			if (PendingScrollOffset >= GetScrollOffsetOfEnd())
			{
				Navigation->Right = UserDefinedNavigation->Right;
				Navigation->Down = UserDefinedNavigation->Down;
			}
			BuildNavigation();
		}
	}
}

UWidget* UModioCommonScrollBox::HandleCustomBoundaryNavigation_Implementation(EUINavigation InNavigation)
{
	if (UModioCommonScrollBoxStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		const float NewOffset = [this, StyleCDO, InNavigation]() {
			if (InNavigation == EUINavigation::Down || InNavigation == EUINavigation::Right)
			{
				return GetScrollOffset() + StyleCDO->NavigationScrollOffsetStep;
			}
			if (InNavigation == EUINavigation::Up || InNavigation == EUINavigation::Left)
			{
				return GetScrollOffset() - StyleCDO->NavigationScrollOffsetStep;
			}
			return static_cast<float>(INDEX_NONE);
		}();
		SetScrollOffset(NewOffset);
		UpdateNavigationData(NewOffset);
	}
	return nullptr;
}
