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
#include "ModioSDK.h"
#include "Types/ModioRating.h"

/// I'd love to just use a static_cast after checking that the value is within the range of the target enum
/// but UE4 doesn't allow signed UENUMS at the present time
/// So a manual switch it is
FORCEINLINE Modio::Rating ToModio(const EModioRating In)
{
	switch (In)
	{
		case EModioRating::Negative:
			return Modio::Rating::Negative;
			break;
		case EModioRating::Neutral:
			return Modio::Rating::Neutral;
			break;
		case EModioRating::Positive:
			return Modio::Rating::Positive;
			break;
	}
	checkf(false, TEXT("EModioRating value was outside the range of expected values"));
	return Modio::Rating::Neutral;
}

FORCEINLINE EModioRating ToUnreal(const Modio::Rating In)
{
	switch (In)
	{
		case Modio::Rating::Negative:
			return EModioRating::Negative;
			break;
		case Modio::Rating::Positive:
			return EModioRating::Positive;
			break;
		case Modio::Rating::Neutral:
			return EModioRating::Neutral;
			break;

	}
	checkf(false, TEXT("Modio::Rating value outside of expected range of values"));
	return EModioRating::Neutral;
}