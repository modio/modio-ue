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
#include "Types/ModioHeaderImage.h"

FORCEINLINE FModioHeaderImage ToUnreal(const Modio::HeaderImage& In)
{
	FModioHeaderImage Out;

	Out.Filename = ToUnreal(In.Filename);
	Out.Original = ToUnreal(In.Original);

	return Out;
}