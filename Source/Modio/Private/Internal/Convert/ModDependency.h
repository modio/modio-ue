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

#include "Internal/ModioConvert.h"

FORCEINLINE FModioModDependency ToUnreal(const Modio::ModDependency& In)
{
	FModioModDependency Out;
	Out.ModID = ToUnreal(In.ModID);
	Out.ModName = ToUnreal(In.ModName);
	Out.DateAdded = ToUnrealDateTime(In.DateAdded);
	Out.DateUpdated = ToUnrealDateTime(In.DateUpdated);
	Out.DependencyDepth = In.DependencyDepth;
	Modio::Detail::Logo Logo;
	if (In.FileInfo.has_value())
	{
		Out.FileInfo = ToUnreal(In.FileInfo.value());
	}
	Out.Status = (EModioModServerSideStatus) In.Status;
	Out.Visibility = (EModioObjectVisibilityFlags) In.Visibility;
	return Out;
}