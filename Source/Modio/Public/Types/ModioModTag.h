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

#include "Containers/UnrealString.h"

#include "ModioModTag.generated.h"

/**
* Strong type struct to wrap a Mod tag
**/
USTRUCT(BlueprintType)
struct MODIO_API FModioModTag
{
	GENERATED_BODY()

	/**
	* A string that represents a mod tag
	**/
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModTag")
	FString Tag;

	bool operator==(const FModioModTag& Other) const
	{
		return Tag == Other.Tag;
	}

	friend uint32 GetTypeHash(const FModioModTag& Other)
	{
		return GetTypeHash(Other.Tag);
	}
};
