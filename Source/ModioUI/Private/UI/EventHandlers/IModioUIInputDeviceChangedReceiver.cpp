/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIInputDeviceChangedReceiver.h"

void IModioUIInputDeviceChangedReceiver::InputDeviceChangedHandler(EModioUIInputMode InputDevice)
{
	bRoutedInputDeviceChanged = false;
	NativeOnInputDeviceChanged(InputDevice);
	checkf(bRoutedInputDeviceChanged, TEXT("Implementing NativeOnInputDeviceChanged requires you to call "
										   "Super::NativeOnInputDeviceChanged to route events to Blueprint"));
}

void IModioUIInputDeviceChangedReceiver::NativeOnInputDeviceChanged(EModioUIInputMode InputDevice)
{
	bRoutedInputDeviceChanged = true;
	Execute_OnInputDeviceChanged(Cast<UObject>(this), InputDevice);
}