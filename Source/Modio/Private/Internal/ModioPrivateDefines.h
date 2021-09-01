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

// This makes missing a value in the switch statement a fatal error, is disabled after our conversion routines
#define MODIO_BEGIN_CONVERT_SWITCHES \
	__pragma(warning(push)) \
	__pragma(warning(error : 4062))

#define MODIO_END_CONVERT_SWITCHES \
	__pragma(warning(pop))
