/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/ScrollBar/ModioCommonScrollBar.h"

#include "UI/Foundation/Components/ScrollBar/ModioCommonScrollBarStyle.h"
#include "Misc/EngineVersionComparison.h"

void UModioCommonScrollBar::SetStyle(TSubclassOf<UModioCommonScrollBarStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonScrollBar::SynchronizeProperties()
{
	if (UModioCommonScrollBarStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
#if UE_VERSION_NEWER_THAN(5, 2, 0)
		SetWidgetStyle(StyleCDO->Style);
		SetAlwaysShowScrollbar(StyleCDO->bAlwaysShowScrollbar);
		SetAlwaysShowScrollbarTrack(StyleCDO->bAlwaysShowScrollbarTrack);
		if (!MyScrollBar.IsValid())
		{
			InitOrientation(StyleCDO->Orientation);
		}
		SetThickness(StyleCDO->Thickness);
		SetPadding(StyleCDO->Padding);
#else
		WidgetStyle = StyleCDO->Style;
		bAlwaysShowScrollbar = StyleCDO->bAlwaysShowScrollbar;
		bAlwaysShowScrollbarTrack = StyleCDO->bAlwaysShowScrollbarTrack;
		Orientation = StyleCDO->Orientation;
		Thickness = StyleCDO->Thickness;
		Padding = StyleCDO->Padding;
#endif
	}
	Super::SynchronizeProperties();
}
