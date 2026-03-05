/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "ModioSDK.h"

#include "Internal/Convert/Rating.h"
#include "Types/ModioUserModRating.h"

FORCEINLINE FModioUserModRating ToUnreal(const Modio::UserRating& In)
{
	FModioUserModRating Out;
	Out.GameId = ToUnreal(In.GameId);
	Out.ModId = ToUnreal(In.ModId);
	Out.Rating = ToUnreal(In.ModRating);
	return Out;
}

FORCEINLINE FModioUserModRatingList ToUnreal(const Modio::UserRatingList& In)
{
	return FModioUserModRatingList(In);
}