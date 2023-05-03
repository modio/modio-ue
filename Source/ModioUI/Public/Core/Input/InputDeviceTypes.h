/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Misc/EnumRange.h"

#include "InputDeviceTypes.generated.h"

/**
* Enumerator with the types of inputs the UI can handle
**/
UENUM(BlueprintType)
enum class EModioUIInputMode : uint8
{
	/** No known input type **/
	Unknown = 0,

	/** Mouse input type **/
	Mouse = 1 << 1,

	/** Keyboard input type **/
	Keyboard = 1 << 2,

	/** Xbox controller input type **/
	XBox = 1 << 3,

	/** Playstation controller input type **/
	Playstation = 1 << 4,

	/** Any controller input type **/
	Controller = XBox | Playstation,

	NintendoSwitch = 1 << 6
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputDeviceChanged, EModioUIInputMode);