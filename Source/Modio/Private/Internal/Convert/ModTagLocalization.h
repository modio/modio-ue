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
#include "Types/ModioModTagInfo.h"

FORCEINLINE FModioModTagLocalizationData ToUnreal(const Modio::ModTagLocalizationData& In)
{
	FModioModTagLocalizationData Result;

	Result.Tag = ToUnreal(In.Tag);

	for (const auto& [key, value] : In.Translations)
	{
		Result.Translations.Add(ToUnreal(key), ToUnrealText(value));
	}

	return Result;
};
