/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Interfaces/IModioUIInputHintDisplayWidget.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"

void IModioUIInputHintDisplayWidget::DisplayHintForInputHandler(FKey VirtualKey)
{
	bRoutedDisplayHintForInput = false;
	NativeDisplayHintForInput(VirtualKey);
	checkf(bRoutedDisplayHintForInput,
		   TEXT("Please call IModioUIDisplayHintForInputReceiver::NativeDisplayHintForInput in your derived "
				"implementation in order to route notifications to blueprint."));
}

void IModioUIInputHintDisplayWidget::NativeDisplayHintForInput(FKey VirtualKey)
{
	bRoutedDisplayHintForInput = true;
	Execute_OnDisplayHintForInput(Cast<UObject>(this), VirtualKey);
}