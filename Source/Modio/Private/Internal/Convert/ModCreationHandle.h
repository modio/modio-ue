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
#include "ModioSDK.h"
#include "Types/ModioModCreationHandle.h"

FORCEINLINE FModioModCreationHandle ToUnreal(const Modio::ModCreationHandle& In)
{
	FModioModCreationHandle Out;
	Out.Underlying = ToUnreal(*In);
	return Out;
}

FORCEINLINE Modio::ModCreationHandle ToModio(const FModioModCreationHandle& In)
{
    return Modio::ModCreationHandle(In.Underlying); 
}