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

#include "ModioCreateModParams.generated.h"

/**
* Strong type for the parameters needed to create a mod file
**/
USTRUCT(BlueprintType)
struct MODIO_API FModioCreateModParams
{
	GENERATED_BODY()

	/**
	* The file system path to the file used as the mod logo
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|ModParams")
	FString PathToLogoFile;
	
	/**
	* The name for the mod to create
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|ModParams")
	FString Name;

	/**
	* Brief description for the mod to create
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|ModParams")
	FString Summary;

	/**
	* Optional override for the name 'slug' in the mod's URL
	**/
	TOptional<FString> NamePath;

	/**
	* Boolean parameter to signal the backend if the mod to upload would be publicly visible. When
	* it is false, the mod will remain hidden
	**/
	TOptional<bool> bVisible;
	
	/**
	* Detailed description for the mod, which can include details such as 'About', 'Features', 
	* 'Install Instructions', 'FAQ', among others
	**/
	TOptional<FString> Description;
	
	/**
	* External webpage for the mod to create
	**/
	TOptional<FString> HomepageURL;
	
	/**
	* If the mod is limited in quantities, set the distribution maximum number
	**/
	TOptional<int64> QuantityAvailable;
	
	/**
	* Any flags that represent mature content for the mod to create
	**/
	TOptional<uint8> MaturityFlags;
	
	/**
	* Extra information for the mod to create
	**/
	TOptional<FString> MetadataBlob;
	
	/**
	* List of metadata tags that reference the mod to create. They help to search and filter content
	**/
	TOptional<TArray<FString>> Tags;
};