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
#include "Types/ModioUser.h"
#include "Types/ModioCommonTypes.h"

FORCEINLINE FModioUser ToUnreal(const Modio::User& In) {
	FModioUser Out;
	Out.UserId = FModioUserID(ToUnreal(In.UserId));
	Out.Username = ToUnreal(In.Username);
	Out.DateOnline = ToUnreal(In.DateOnline);
	Out.ProfileUrl = ToUnreal(In.ProfileUrl);
	Out.DisplayNamePortal = ToUnreal(In.DisplayNamePortal);
	return Out;
}