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

#if defined(_MSC_VER)

	#ifdef MODIO_USE_CHECK_SWITCHES
	// This makes missing a value in the switch statement a fatal error, is disabled after our conversion routines
	#define MODIO_BEGIN_CONVERT_SWITCHES \
			__pragma(warning(push)) \
			// Compiler warning (level 4) C4062	enumerator 'identifier' in switch of enum 'enumeration' is not handled
			__pragma(warning(error : 4062))

	#define MODIO_END_CONVERT_SWITCHES \
			__pragma(warning(pop))
	#else
		#define MODIO_BEGIN_CONVERT_SWITCHES
		#define MODIO_END_CONVERT_SWITCHES
	#endif

#elif defined(__GNUC__) || defined(__clang__)

	#define MODIO_BEGIN_CONVERT_SWITCHES
	#define MODIO_END_CONVERT_SWITCHES

#else

	#define MODIO_BEGIN_CONVERT_SWITCHES
	#define MODIO_END_CONVERT_SWITCHES

#endif
