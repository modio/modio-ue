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
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"

FORCEINLINE FModioModMonetizationSKU ToUnreal(const Modio::ModMonetizationSKU& In)
{
	FModioModMonetizationSKU Out {};
	Out.Id = static_cast<int64>(In.Id);
	Out.SKUName = ToUnreal(In.Sku);
	Out.Portal = ToUnreal(In.Portal);
	return Out;
}
