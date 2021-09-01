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
#include "Types/ModioUser.h"
#include "Types/ModioCommonTypes.h"

FORCEINLINE FModioUser ToUnreal(const Modio::User& In) {
	FModioUser Out;
	Out.UserId = FModioUserID(ToUnreal(In.UserId));
	Out.Username = ToUnreal(In.Username);
	Out.DateOnline = ToUnreal(In.DateOnline);
	Out.ProfileUrl = ToUnreal(In.ProfileUrl);
	return Out;
}