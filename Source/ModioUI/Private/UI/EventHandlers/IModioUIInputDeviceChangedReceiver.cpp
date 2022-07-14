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