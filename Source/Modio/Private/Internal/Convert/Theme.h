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

#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioTheme.h"

FORCEINLINE FModioTheme ToUnreal(const Modio::Theme& In)
{
	FModioTheme Out;

	Out.Primary = ToUnreal(In.Primary);
	Out.Dark = ToUnreal(In.Dark);
	Out.Light = ToUnreal(In.Light);
	Out.Success = ToUnreal(In.Success);
	Out.Warning = ToUnreal(In.Warning);
	Out.Danger = ToUnreal(In.Danger);

	return Out;
}
