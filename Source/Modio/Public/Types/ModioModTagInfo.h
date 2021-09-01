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

#include "Containers/Array.h"
#include "Containers/UnrealString.h"

#include "ModioModTagInfo.generated.h"

/** @brief Metadata about a group of tags that can be used for filtering mods */
USTRUCT(BlueprintType)
struct MODIO_API FModioModTagInfo
{
	GENERATED_BODY()

	/** @brief The display name for the tag */
	UPROPERTY(BlueprintReadOnly,Category="mod.io|ModTagInfo")
	FString TagGroupName;

	/** @brief The valid tags the group can have */
	UPROPERTY(BlueprintReadOnly,Category="mod.io|ModTagInfo")
	TArray<FString> TagGroupValues;

	/** @brief True if multiple tags from the group can be used simultaneously */
	UPROPERTY(BlueprintReadOnly,Category="mod.io|ModTagInfo")
	bool bAllowMultipleSelection;
};

