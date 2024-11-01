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

	/**
	 * A localized tag string
	 **/
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModTag")
	FText TagLocalized;

	FModioModTag()
	{
		Tag = "";
		TagLocalized = FText();
	}

	FModioModTag(const FString& InTag)
	{
		Tag = InTag;
		TagLocalized = FText();
	}

	FModioModTag(const FString& InTag, const FText& InTagLoc)
	{
		Tag = InTag;
		TagLocalized = InTagLoc;
	}

	bool operator==(const FModioModTag& Other) const
	{
		return Tag == Other.Tag;
	}

	friend uint32 GetTypeHash(const FModioModTag& Other)
	{
		return GetTypeHash(Other.Tag);
	}
};
