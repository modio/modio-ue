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
#include "Types/ModioModTag.h"

FORCEINLINE FModioModTag ToUnreal(const Modio::ModTag& In)
{
	FModioModTag Out;
	Out.Tag = ToUnreal(In.Tag);
	Out.TagLocalized = ToUnrealText(In.TagLocalized);

	return Out;
}
