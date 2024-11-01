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

#include "Internal/Convert/Optional.h"
#include "Internal/ModioConvert.h"
#include "Types/ModioMetadata.h"
#include "ModioSDK.h"

FORCEINLINE FModioMetadata ToUnreal(const Modio::Metadata& In)
{
	FModioMetadata Out;
	Out.Key = ToUnreal(In.Key);
	Out.Value = ToUnreal(In.Value);
	return Out;
}