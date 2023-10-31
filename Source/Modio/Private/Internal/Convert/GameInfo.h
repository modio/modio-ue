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

FORCEINLINE FModioGameInfo ToUnreal(const Modio::GameInfo& In)
{
	FModioGameInfo Out = {};

	Out.GameID = ToUnreal(In.GameID);
	Out.DateAdded = ToUnrealDateTime(In.DateAdded);
	Out.DateUpdated = ToUnrealDateTime(In.DateUpdated);
	Out.DateLive = ToUnrealDateTime(In.DateLive);
	Out.UgcName = ToUnreal(In.UgcName);
	Out.Icon = ToUnreal(In.Icon);
	Out.Logo = ToUnreal(In.Logo);
	Out.HeaderImage = ToUnreal(In.HeaderImage);
	Out.Name = ToUnreal(In.Name);
	Out.Summary = ToUnreal(In.Summary);
	Out.Instructions = ToUnreal(In.Instructions);
	Out.InstructionsUrl = ToUnreal(In.InstructionsUrl);
	Out.ProfileUrl = ToUnreal(In.ProfileUrl);
	Out.Theme = ToUnreal(In.Theme);
	Out.Stats = ToUnreal(In.Stats);
	Out.OtherUrls = ToUnreal<FModioOtherUrl>(In.OtherUrls);
	Out.Platforms = ToUnreal<EModioModfilePlatform>(In.Platforms);
	Out.GameMonetizationOptions = ToUnreal<EGameMonetizationFlags, Modio::GameMonetizationOptions>(In.GameMonetizationOptions);
	Out.VirtualTokenName = ToUnreal(In.VirtualTokenName);
	Out.PlatformSupport = ToUnreal<FModioGamePlatform>(In.PlatformSupport);

	return Out;
}
