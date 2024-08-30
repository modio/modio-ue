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


/** @brief Localization data for an individual tag value */
USTRUCT(BlueprintType)
struct MODIO_API FModioModTagLocalizationData
{
	GENERATED_BODY()

	/** @brief The original raw unlocalized tag value used by the REST API */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	FString Tag;

	/** @brief Culture code -> Localized tag value string mapping for all configured languages. */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	TMap<FString, FText> Translations;

	FModioModTagLocalizationData()
	{
		Tag = "";
		Translations = TMap<FString, FText>();
	}

	FModioModTagLocalizationData(FString InTag)
	{
		Tag = InTag;
		Translations = TMap<FString, FText>();
	}
};

/**
 * @brief Struct containing pre-localized display strings for a tag group
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioLocalizedTagCategory
{
	GENERATED_BODY()

	/** @brief Localized display string for this tag category's name */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	FString GroupName;

	/** @brief Localized display strings for all valid values in this tag category */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	TArray<FString> Tags;

	FModioLocalizedTagCategory()
	{
		GroupName = "";
		Tags = TArray<FString>();
	}

	FModioLocalizedTagCategory(FString InGroupName, TArray<FString> InTags)
	{
		GroupName = InGroupName;
		Tags = InTags;
	}

};

/**
 * Metadata about a group of tags that can be used for filtering mods
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioModTagInfo
{
	GENERATED_BODY()

	/** @brief Raw unlocalized tag group name */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo|Unlocalized")
	FString TagGroupName;

	/** @brief Valid raw unlocalized tag values this group contains */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo|Unlocalized")
	TArray<FString> TagGroupValues;

	/** @brief Culture code -> localized tag category name mapping for all configured languages */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo|Localization Data")
	TMap<FString, FText> TagGroupNameLocalizationData; //-> TagGroupNameLocData

	/** @brief Localization data for this tag category's values in all configured languages */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo|Localization Data")
	TArray<FModioModTagLocalizationData> TagGroupValueLocalizationData;

	/** @brief True if multiple tags from the group can be used simultaneously */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	bool bAllowMultipleSelection {};

	/** @brief True if only visible by admins */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	bool bHidden {};

	/** @brief TrueTrue if only editable by admins */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo")
	bool bLocked {};

	/** @brief the culture code for the mod.io locale at the time the mod.io API returned this data */
	UPROPERTY()
	FString Locale;

	/** @brief Localized display string for this tag category and its values in the current mod.io locale set at the
	 * time this data was requested from the API */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "mod.io|ModTagInfo|Localized")
	FModioLocalizedTagCategory LocalizedTagCategoryDataCurrentLocale;
};
