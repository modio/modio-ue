/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/List/ModioCommonListView.h"
#include "UI/Foundation/Components/List/ModioCommonListViewStyle.h"

void UModioCommonListView::SetStyle(TSubclassOf<UModioCommonListViewStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

void UModioCommonListView::SynchronizeProperties()
{
	if (UModioCommonListViewStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		WidgetStyle = StyleCDO->WidgetStyle;
		ScrollBarStyle = StyleCDO->ScrollBarStyle;
		Orientation = StyleCDO->Orientation;
		SelectionMode = StyleCDO->SelectionMode;
	}

	Super::SynchronizeProperties();
}
