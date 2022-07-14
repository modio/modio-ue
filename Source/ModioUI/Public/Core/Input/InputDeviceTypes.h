#pragma once

#include "Misc/EnumRange.h"

#include "InputDeviceTypes.generated.h"

UENUM(BlueprintType)
enum class EModioUIInputMode : uint8
{
	Unknown = 0,
	Mouse = 1 << 1,
	Keyboard = 1 << 2,
	XBox = 1 << 3,
	Playstation = 1 << 4,
	Controller = XBox | Playstation
};


DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputDeviceChanged, EModioUIInputMode);