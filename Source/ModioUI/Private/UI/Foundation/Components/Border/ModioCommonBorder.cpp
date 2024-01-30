/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Border/ModioCommonBorder.h"

#include "UI/Foundation/Components/Border/ModioCommonBorderStyle.h"

UModioCommonBorder::UModioCommonBorder()
{
	OnMouseButtonDownEvent.BindDynamic(this, &UModioCommonBorder::HandleOnMouseButtonDownEvent);
}

void UModioCommonBorder::SetStyle(TSubclassOf<UModioCommonBorderStyle> InStyle)
{
	TSubclassOf<UCommonBorderStyle> CommonStyle = InStyle;
	Super::SetStyle(CommonStyle);
}

FEventReply UModioCommonBorder::HandleOnMouseButtonDownEvent(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (OnModioCommonBorderClicked.IsBound())
	{
		OnModioCommonBorderClicked.Broadcast();
		return FEventReply(true);
	}
	return FEventReply(false);
}
