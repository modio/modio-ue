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

#include "Internal/Convert/FilterParams.h"
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioFilterParams.h"
#include "Types/ModioPlacement.h"

inline EModioPlacementType ToUnreal(const Modio::Placement::PlacementType& In)
{
	switch (In)
	{
		case Modio::Placement::PlacementType::Filters:
			return EModioPlacementType::Filters;
		default:
			return EModioPlacementType::Filters;
	}
}

inline EModioPlacementSize ToUnreal(const Modio::Placement::PlacementSize& In)
{
	switch (In)
	{
		case Modio::Placement::PlacementSize::Small:
			return EModioPlacementSize::Small;
		case Modio::Placement::PlacementSize::Medium:
			return EModioPlacementSize::Medium;
		case Modio::Placement::PlacementSize::Large:
			return EModioPlacementSize::Large;
		default:
			return EModioPlacementSize::Small;
	}
}

FORCEINLINE FModioPlacement ToUnreal(const Modio::Placement& In)
{
	FModioPlacement Out;
	Out.ID = ToUnreal(In.ID);
	Out.Type = ToUnreal(In.Type);
	Out.Name = ToUnreal(In.Name);
	Out.NameLocalized = ToUnreal(In.NameLocalized);
	Out.Filters = ToUnreal(In.Filters);

	Out.DisplayPosition = In.DisplayPosition;
	Out.Total = In.Total;
	Out.Size = ToUnreal(In.Size);
	Out.bEnabled = ToUnreal(In.bEnabled);

	return Out;
}
