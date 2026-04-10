/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "ModioSDK.h"

#include "Types/ModioUserList.h"

FORCEINLINE FModioUserList ToUnreal(const Modio::UserList& In)
{
	return FModioUserList(In);
}