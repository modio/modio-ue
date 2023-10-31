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
#include "Internal/Convert/GameStats.h"
#include "Internal/Convert/HeaderImage.h"
#include "Internal/Convert/Icon.h"
#include "Internal/Convert/Logo.h"
#include "Internal/Convert/OtherUrl.h"
#include "Internal/Convert/Theme.h"
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioGameInfo.h"

FORCEINLINE FModioGamePlatform ToUnreal(const Modio::GamePlatform& In)
{
	FModioGamePlatform Out = {};

	Out.Locked = In.Locked;
	Out.Moderated = In.Moderated;
	Out.Platform = ToUnreal(In.Platform);
	
	return Out;
}
