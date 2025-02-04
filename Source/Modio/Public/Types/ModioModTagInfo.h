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

#include "Containers/Array.h"
#include "Containers/UnrealString.h"

#include "ModioModTagInfo.generated.h"


/**
 * @docpublic
 * @brief Localization data for an individual tag value
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModTagLocalizationData
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief The original raw unlocalized tag value used by the REST API
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	FString Tag;

	/** 
	 * @docpublic
	 * @brief Culture code -> Localized tag value string mapping for all configured languages.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	TMap<FString, FText> Translations;

	/** 
	 * @docpublic
	 * @brief Default constructor initializes Tag to an empty string and Translations to an empty map
	 */
	FModioModTagLocalizationData()
	{
		Tag = "";
		Translations = TMap<FString, FText>();
	}

	/** 
	 * @docpublic
	 * @brief Constructor that initializes Tag with the specified value and sets Translations to an empty map
	 * @param InTag The tag string
	 */
	FModioModTagLocalizationData(FString InTag)
	{
		Tag = InTag;
		Translations = TMap<FString, FText>();
	}
};

/**
 * @docpublic
 * @brief Struct containing pre-localized display strings for a tag group
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioLocalizedTagCategory
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief Localized display string for this tag category's name
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	FString GroupName;

	/** 
	 * @docpublic
	 * @brief Localized display strings for all valid values in this tag category
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	TArray<FString> Tags;

	/** 
	 * @docpublic
	 * @brief Default constructor initializes GroupName to an empty string and Tags to an empty array
	 */
	FModioLocalizedTagCategory()
	{
		GroupName = "";
		Tags = TArray<FString>();
	}

	/** 
	 * @docpublic
	 * @brief Constructor that initializes GroupName and Tags with the specified values
	 * @param InGroupName The group name
	 * @param InTags The array of tags for this group
	 */
	FModioLocalizedTagCategory(FString InGroupName, TArray<FString> InTags)
	{
		GroupName = InGroupName;
		Tags = InTags;
	}
};

/**
 * @docpublic
 * @brief Metadata about a group of tags that can be used for filtering mods
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModTagInfo
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief Raw unlocalized tag group name
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo|Unlocalized")
	FString TagGroupName;

	/** 
	 * @docpublic
	 * @brief Valid raw unlocalized tag values this group contains
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo|Unlocalized")
	TArray<FString> TagGroupValues;

	/** 
	 * @docpublic
	 * @brief Culture code -> localized tag category name mapping for all configured languages
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo|Localization Data")
	TMap<FString, FText> TagGroupNameLocalizationData; //-> TagGroupNameLocData

	/** 
	 * @docpublic
	 * @brief Localization data for this tag category's values in all configured languages
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo|Localization Data")
	TArray<FModioModTagLocalizationData> TagGroupValueLocalizationData;

	/** 
	 * @docpublic
	 * @brief True if multiple tags from the group can be used simultaneously
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	bool bAllowMultipleSelection {};

	/** 
	 * @docpublic
	 * @brief True if only visible by admins
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	bool bHidden {};

	/** 
	 * @docpublic
	 * @brief True if only editable by admins
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	bool bLocked {};

	/** 
	 * @docpublic
	 * @brief The culture code for the mod.io locale at the time the mod.io API returned this data
	 */
	UPROPERTY()
	FString Locale;

	/** 
	 * @docpublic
	 * @brief Localized display string for this tag category and its values in the current mod.io locale set at the time this data was requested from the API
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo|Localized")
	FModioLocalizedTagCategory LocalizedTagCategoryDataCurrentLocale;
};