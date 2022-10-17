/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Core/Input/ModioUIInputProcessor.h"
#include "Core/Input/ModioInputKeys.h"
#include "Input/Events.h"

EModioUIInputMode FModioUIInputProcessor::DetectDeviceType(const FKeyEvent& InEvent)
{
	if (InEvent.GetKey().IsGamepadKey())
	{
		if (CurrentControllerTypeOverride.IsSet())
		{
			return CurrentControllerTypeOverride.GetValue();
		}
		return EModioUIInputMode::Controller;
	}
	else
	{
		return EModioUIInputMode::Keyboard;
	}
}

bool FModioUIInputProcessor::ShouldEmitDeviceChangeEventsForKeyEvent(const FKeyEvent& InEvent)
{
	// Only emit device change events for keys that aren't our virtual keys
	return !FModioInputKeys::GetAll().Contains(InEvent.GetKey());
}

void FModioUIInputProcessor::SetControllerOverrideType(EModioUIInputMode Override)
{
	if (Override == EModioUIInputMode::Playstation || Override == EModioUIInputMode::XBox)
	{
		CurrentControllerTypeOverride = Override;
	}
	else
	{
		CurrentControllerTypeOverride.Reset();
	}
}
