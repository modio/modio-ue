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
#include "Types/ModioOtherUrl.h"

FORCEINLINE FModioOtherUrl ToUnreal(const Modio::OtherUrl& In)
{
	FModioOtherUrl Out = {};

	Out.Label = ToUnreal(In.Label);
	Out.Url = ToUnreal(In.Url);

	return Out;
}