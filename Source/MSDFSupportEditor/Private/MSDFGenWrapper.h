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
#if PLATFORM_WINDOWS
	#include "Windows/PreWindowsApi.h"
#endif
#pragma warning(push)

THIRD_PARTY_INCLUDES_START

#pragma warning(disable : 4191)
#pragma warning(disable : 4456)
#include "msdfgen.h"
#include "msdfgen-ext.h"
THIRD_PARTY_INCLUDES_END
#pragma warning(pop)


#if PLATFORM_WINDOWS
#include "Windows/PostWindowsApi.h"
#endif
