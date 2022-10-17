/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Interfaces/IModioUIStringInputWidget.h"

void IModioUIStringInputWidget::SetHint_Implementation(const FText& InHintText)
{
	NativeSetHintText(InHintText);
}

FString IModioUIStringInputWidget::GatherInput_Implementation()
{
	return NativeGatherInput();
}

void IModioUIStringInputWidget::SetInput_Implementation(const FString& Input)
{
	NativeSetInput(Input);
}
