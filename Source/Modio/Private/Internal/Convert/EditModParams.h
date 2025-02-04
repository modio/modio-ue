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

#include "Internal/Convert/Numerics.h"
#include "Internal/Convert/Optional.h"
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioEditModParams.h"

FORCEINLINE Modio::EditModParams ToModio(const FModioEditModParams& In)
{
	Modio::EditModParams Out;

	Out.Name = ToModioOptional<std::string>(In.Name);
	Out.Summary = ToModioOptional<std::string>(In.Summary);
	Out.NamePath = ToModioOptional<std::string>(In.NamePath);
	Out.Visibility = In.Visibility.IsSet() ? static_cast<Modio::ObjectVisibility>(In.Visibility.GetValue())
										   : Modio::Optional<Modio::ObjectVisibility> {};
	Out.Description = ToModioOptional<std::string>(In.Description);
	Out.HomepageURL = ToModioOptional<std::string>(In.HomepageURL);
	Out.MaturityRating = In.MaturityFlags.IsSet() ? static_cast<Modio::MaturityOption>(In.MaturityFlags.GetValue())
												  : Modio::Optional<Modio::MaturityOption> {};
	Out.CommunityOptions = In.CommunityOptionsFlags.IsSet()
		                       ? ToModio(In.CommunityOptionsFlags.GetValue())
		                       : Modio::Optional<Modio::ModCommunityOptionsFlags>{};
	Out.MetadataBlob = ToModioOptional<std::string>(In.MetadataBlob);
	Out.LogoPath = ToModioOptional<std::string>(In.LogoPath);
	Out.Tags = ToModioOptional<std::vector<std::string>>(In.Tags);
	return Out;
}