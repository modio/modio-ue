/*
 *  Copyright (C) 2025-2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Internal/ModioConvert.h"
#include "Internal/ModioPrivateDefines.h"
#include "Modio.h"
#include "ModioSDK.h"
#include "InitializeOptions.h"
#include "Types/ModioServerInitializeOptions.h"

FORCEINLINE Modio::ServerInitializeOptions ToModio(const FModioServerInitializeOptions& In)
{
	Modio::ServerInitializeOptions Options;
	Options.GameID = ToModio(In.BaseOptions.GameId);
	Options.APIKey = ToModio(In.BaseOptions.ApiKey);
	Options.GameEnvironment = ToModio(In.BaseOptions.GameEnvironment);
	Options.PortalInUse = ToModio(In.BaseOptions.PortalInUse);
#if PLATFORM_WINDOWS
	Options.User = In.BaseOptions.LocalSessionIdentifier.IsSet() ?
		ToModio(In.BaseOptions.LocalSessionIdentifier.GetValue()) : GetUserSidString();
#else
	Options.User = In.BaseOptions.LocalSessionIdentifier.IsSet()
					   ? ToModio(In.BaseOptions.LocalSessionIdentifier.GetValue()) : "unknown-sid";
#endif
	Options.ModsDirectory = ToModio(In.ModsDirectory);
	Options.Token = ToModio(In.Token);
	Options.Mods = ToModio(In.Mods);

	for (const TPair<FString, FString>& ExtParam : In.BaseOptions.ExtendedInitializationParameters)
	{
		Options.ExtendedParameters.emplace(TCHAR_TO_UTF8(*ExtParam.Key), TCHAR_TO_UTF8(*ExtParam.Value));
	}

	return Options;
}