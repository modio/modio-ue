/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioLogo.h"

FORCEINLINE FModioLogo ToUnreal(const Modio::Detail::Logo& In)
{
	FModioLogo Out;

	Out.Filename = ToUnreal(In.Filename);
	Out.Original = ToUnreal(In.Original);
	Out.Thumb320x180 = ToUnreal(In.Thumb320x180);
	Out.Thumb640x360 = ToUnreal(In.Thumb640x360);
	Out.Thumb1280x720 = ToUnreal(In.Thumb1280x720);

	return Out;
}