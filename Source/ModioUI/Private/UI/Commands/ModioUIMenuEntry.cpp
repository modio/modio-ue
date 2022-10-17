/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Commands/ModioUIMenuEntry.h"

uint32 GetTypeHash(const FModioUIMenuEntry& Object)
{
	uint32 Hash = FCrc::MemCrc32(&Object, sizeof(FModioUIMenuEntry));
	return Hash;
}
