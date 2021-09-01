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

//#if PLATFORM_WINDOWS
//	// clang-format off
//	#include "Windows/AllowWindowsPlatformTypes.h"
//	#include "Windows/PreWindowsApi.h"
//	#include "Windows/AllowWindowsPlatformAtomics.h"
//// clang-format on
//#endif

#define MODIO_PLATFORM_UNREAL 1

#pragma push_macro("check")

#undef check

#pragma warning(push)
// These warnings are in third party libraries and we don't care about those
#pragma warning(disable : 4265 4996 4668 4191 4583 4582)
// These warnings is in our code, and should be fixed
#pragma warning(disable : 4063)
#include "modio/ModioSDK.h"



#undef MODIO_PLATFORM_UNREAL
#undef ASIO_NO_EXCEPTIONS

#pragma warning(pop)

#pragma pop_macro("check")

//#if PLATFORM_WINDOWS
//  // clang-format off
//	#include "Windows/PostWindowsApi.h"
//	#include "Windows/HideWindowsPlatformTypes.h"
//	#include "Windows/HideWindowsPlatformAtomics.h"
//  // clang-format on
//#endif
