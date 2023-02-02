/*
 *  Copyright (C) 2021-2022 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "ModioSDK.h"
#include "Types/ModioUser.h"

TArray<FModioUser> ToUnreal(const std::vector<Modio::User>& OriginalArray)
{
	TArray<FModioUser> Result;

	Result.Reserve(OriginalArray.size());
	for (const auto& It : OriginalArray)
	{
		Result.Emplace(ToUnreal(It));
	}

	return Result;
}