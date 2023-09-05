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
}
