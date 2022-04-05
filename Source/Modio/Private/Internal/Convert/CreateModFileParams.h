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

#include "Internal/Convert/Numerics.h"

#include "Internal/Convert/Optional.h"
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioCreateModFileParams.h"

FORCEINLINE Modio::CreateModFileParams ToModio(const FModioCreateModFileParams& In)
{
	Modio::CreateModFileParams Out;
	Out.RootDirectory = ToModio(In.PathToModRootDirectory);
	Out.Version = ToModioOptional<std::string>(In.VersionString);
	Out.Changelog = ToModioOptional<std::string>(In.Changelog);
	Out.bSetAsActive = ToModioOptional<bool>(In.bSetAsActiveRelease);
	Out.MetadataBlob = ToModioOptional<std::string>(In.MetadataBlob);
	return Out;
}