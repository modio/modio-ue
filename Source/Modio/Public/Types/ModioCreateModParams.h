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

#include "ModioModInfo.h"
#include "ModioCreateModParams.generated.h"

/**
 * @docpublic
 * @brief Strong type for the parameters needed to create a mod file
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioCreateModParams
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief The file system path to the file used as the mod logo
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|ModParams")
	FString PathToLogoFile;

	/**
	 * @docpublic
	 * @brief The name for the mod to create
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|ModParams")
	FString Name;

	/**
	 * @docpublic
	 * @brief Brief description for the mod to create
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|ModParams")
	FString Summary;

	/**
	 * @docpublic
	 * @brief Optional override for the name 'slug' in the mod's URL
	 */
	TOptional<FString> NamePath;

	/**
	 * @docpublic
	 * @brief Boolean parameter to signal the backend if the mod to upload would be publicly visible. When
	 * it is false, the mod will remain hidden
	 * @deprecated Use Visibility property instead
	 */
	TOptional<bool> bVisible_DEPRECATED;

	/**
	 * @docpublic
	 * @brief Enum parameter to signal the backend if the mod to upload would be publicly visible. Default 
	 * value is Public
	 */
	TOptional<uint8> Visibility;

	/**
	 * @docpublic
	 * @brief Detailed description for the mod, which can include details such as 'About', 'Features', 
	 * 'Install Instructions', 'FAQ', among others
	 */
	TOptional<FString> Description;

	/**
	 * @docpublic
	 * @brief External webpage for the mod to create
	 */
	TOptional<FString> HomepageURL;

	/**
	 * @docpublic
	 * @brief If the mod is limited in quantities, set the distribution maximum number
	 */
	TOptional<int64> QuantityAvailable;

	/**
	 * @docpublic
	 * @brief Any flags that represent mature content for the mod to create
	 */
	TOptional<EModioMaturityFlags> MaturityFlags;

	/**
	 * Any flags that represent community options for the mod to create
	 */
	TOptional<EModioModCommunityOptionsFlags> CommunityOptionsFlags;

	/**
	 * @docpublic
	 * @brief Extra information for the mod to create
	 */
	TOptional<FString> MetadataBlob;

	/**
	 * @docpublic
	 * @brief List of metadata tags that reference the mod to create. They help to search and filter content
	 */
	TOptional<TArray<FString>> Tags;
};