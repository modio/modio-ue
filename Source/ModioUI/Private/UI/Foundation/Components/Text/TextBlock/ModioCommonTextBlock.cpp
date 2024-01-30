/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlockStyle.h"

void UModioCommonTextBlock::SetStyle(TSubclassOf<UModioCommonTextStyle> InStyle)
{
	TSubclassOf<UCommonTextStyle> CommonStyle = InStyle;
	Super::SetStyle(MoveTemp(CommonStyle));
}
