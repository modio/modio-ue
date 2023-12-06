/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/ProgressBar/ModioCommonProgressBar.h"

#include "UI/Foundation/Components/ProgressBar/ModioCommonProgressBarStyle.h"
#include "Misc/EngineVersionComparison.h"

void UModioCommonProgressBar::SetStyle(TSubclassOf<UModioCommonProgressBarStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		if (MyProgressBar)
		{
			SynchronizeProperties();
		}
	}
}

void UModioCommonProgressBar::SynchronizeProperties()
{
	if (ModioStyle)
	{
		UModioCommonProgressBarStyle* StyleCDO = ModioStyle.GetDefaultObject();
#if UE_VERSION_NEWER_THAN(5, 1, 0)
		SetWidgetStyle(StyleCDO->Style);
#else
		WidgetStyle = StyleCDO->Style;
#endif
	}
	Super::SynchronizeProperties();
}
