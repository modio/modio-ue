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
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioModTagInfo.h"

FORCEINLINE FModioModTagInfo ToUnreal(const Modio::ModTagInfo& In)
{
	FModioModTagInfo Out;
	Out.TagGroupName = ToUnreal(In.TagGroupName);
	Out.TagGroupValues = ToUnreal<FString>(In.TagGroupValues);
	Out.bAllowMultipleSelection = ToUnreal(In.bAllowMultipleSelection);

	for(const auto& Value : In.TagGroupValueLocData)
	{
		Out.TagGroupValueLocalizationData.Add(ToUnreal(Value));
	}

	for (const auto& [Key, Value] : In.TagGroupNameLocData)
	{
		Out.TagGroupNameLocalizationData.Add(ToUnreal(Key), ToUnrealText(Value));
	}

	Modio::LocalizedTagCategory LocTags = In.GetLocalizedTags();

	Out.LocalizedTagCategoryDataCurrentLocale.GroupName = ToUnreal(LocTags.GroupName);

	for (const auto& Value : LocTags.Tags)
	{
		Out.LocalizedTagCategoryDataCurrentLocale.Tags.Add(ToUnreal(Value));
	}

	return Out;
}
