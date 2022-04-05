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
#include "Internal/Convert/Optional.h"
#include "Internal/Convert/Numerics.h"
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioCreateModParams.h"

FORCEINLINE Modio::CreateModParams ToModio(const FModioCreateModParams& In)
{
	Modio::CreateModParams Out;
	Out.PathToLogoFile = ToModio(In.PathToLogoFile);
	Out.Name = ToModio(In.Name);
	Out.Summary = ToModio(In.Summary);
	Out.NamePath = ToModioOptional<std::string>(In.NamePath);
	Out.bVisible = ToModioOptional<bool>(In.bVisible);
	Out.Description = ToModioOptional<std::string>(In.Description);
	Out.HomepageURL = ToModioOptional<std::string>(In.HomepageURL);
	Out.Stock = ToModioOptional<int64>(In.QuantityAvailable);
	Out.MaturityRating = In.MaturityFlags.IsSet() ? static_cast<Modio::MaturityOption>(In.MaturityFlags.GetValue()): Modio::Optional<Modio::MaturityOption>{};
	Out.MetadataBlob = ToModioOptional<std::string>(In.MetadataBlob);
	Out.Tags = ToModioOptional<std::vector<std::string>>(In.Tags);
    return Out;
}