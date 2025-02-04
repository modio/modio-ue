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
 * @docpublic
 * @brief Strong type struct to wrap a Mod tag
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModTag
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief A string that represents a mod tag
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModTag")
	FString Tag;

	/** 
	 * @docpublic
	 * @brief A localized tag string
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModTag")
	FText TagLocalized;

	/** 
	 * @docpublic
	 * @brief Default constructor initializes Tag to an empty string and TagLocalized to an empty FText
	 */
	FModioModTag()
	{
		Tag = "";
		TagLocalized = FText();
	}

	/** 
	 * @docpublic
	 * @brief Constructor that initializes Tag with the specified string, TagLocalized is empty
	 * @param InTag The tag string
	 */
	FModioModTag(const FString& InTag)
	{
		Tag = InTag;
		TagLocalized = FText();
	}

	/** 
	 * @docpublic
	 * @brief Constructor that initializes both Tag and TagLocalized with the specified values
	 * @param InTag The tag string
	 * @param InTagLoc The localized tag string
	 */
	FModioModTag(const FString& InTag, const FText& InTagLoc)
	{
		Tag = InTag;
		TagLocalized = InTagLoc;
	}

	/** 
	 * @docpublic
	 * @brief Compares two FModioModTag objects for equality
	 * @param Other Another FModioModTag object to compare against
	 * @return True if both Tag values are equal, otherwise false
	 */
	bool operator==(const FModioModTag& Other) const
	{
		return Tag == Other.Tag;
	}

	friend uint32 GetTypeHash(const FModioModTag& Other)
	{
		return GetTypeHash(Other.Tag);
	}
};