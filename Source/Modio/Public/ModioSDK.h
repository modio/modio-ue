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

#define MODIO_PLATFORM_UNREAL 1


#if defined(_MSC_VER)
    #pragma warning(push)
    // These warnings are in third party libraries and we don't care about those
    #pragma warning(disable : 4265 4996 4668 4191 4583 4582)
    // These warnings is in our code, and should be fixed
    #pragma warning(disable : 4063)
#endif

#include "modio/ModioSDK.h"

#undef MODIO_PLATFORM_UNREAL
#undef ASIO_NO_EXCEPTIONS

#if defined(_MSC_VER)
    #pragma warning(pop)
#endif
