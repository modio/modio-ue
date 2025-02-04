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
#include "Types/ModioModInfo.h"
#include "Misc/Optional.h"

#include "ModioEditModParams.generated.h"

/**
 * @docpublic
 * @brief Struct type to specify the fields to update for a mod
 */
USTRUCT(BlueprintType)
struct FModioEditModParams
{
	GENERATED_BODY()

public:

	/**
	 * @docpublic
	 * @brief The name of the mod
	 */
	TOptional<FString> Name;
    
	/**
	 * @docpublic
	 * @brief A brief summary of what the mod is
	 */
	TOptional<FString> Summary;
    
	/**
	 * @docpublic
	 * @brief Optional override for the name 'slug' in the mod's URL
	 */
	TOptional<FString> NamePath;
    
	/**
	 * @docpublic
	 * @brief Optional override for the mod's visibility status. Defaults to true (visible)
	 * @deprecated Use Visibility property instead
	 */
	TOptional<bool> bVisible_DEPRECATED;
    
	/**
	 * @docpublic
	 * @brief Enum parameter to signal the backend if the mod to upload would be publicly visible. Default value is Public
	 */
	TOptional<EModioObjectVisibilityFlags> Visibility;
    
	/**
	 * @docpublic
	 * @brief Optional long description of the mod
	 */
	TOptional<FString> Description;
    
	/**
	 * @docpublic
	 * @brief Optional URL to the mod's homepage. Must be a valid URL
	 */
	TOptional<FString> HomepageURL;
    
	/**
	 * @docpublic
	 * @brief Bitwise mask of flags indicating the type of mature content present in this mod
	 */
	TOptional<EModioMaturityFlags> MaturityFlags;

	/**
	 * Bitwise mask of flags indicating the community options for this mod
	 */
	TOptional<EModioModCommunityOptionsFlags> CommunityOptionsFlags;
    
	/**
	 * @docpublic
	 * @brief Optional metadata blob for this mod
	 */
	TOptional<FString> MetadataBlob;

	/**
	 * @docpublic
	 * @brief Optional path to a new logo image for this mod
	 */
	TOptional<FString> LogoPath;

	/**
	 * @docpublic
	 * @brief List of metadata tags that reference the mod to create. They help to search and filter content
	 */
	TOptional<TArray<FString>> Tags;
};