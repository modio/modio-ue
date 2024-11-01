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

#include "Types/ModioErrorCode.h"
#include "ModioSDK.h"

FModioErrorCode ToUnreal(Modio::ErrorCode ec)
{
	return FModioErrorCode(ec);
}