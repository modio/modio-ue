/*
 *  Copyright (C) 2021-2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioIcon.h"

FORCEINLINE FModioIcon ToUnreal(const Modio::Detail::Icon& In)
{
	FModioIcon Out;

	Out.Filename = ToUnreal(In.Filename);
	Out.Original = ToUnreal(In.Original);
	Out.Thumb64x64 = ToUnreal(In.Thumb64x64);
	Out.Thumb128x128 = ToUnreal(In.Thumb128x128);
	Out.Thumb256x256 = ToUnreal(In.Thumb256x256);

	return Out;
}