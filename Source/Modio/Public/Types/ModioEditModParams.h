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
#include "Types/ModioModInfo.h"
#include "Misc/Optional.h"

#include "ModioEditModParams.generated.h"

/**
* Struct type to specify the fields to update for a mod
**/
USTRUCT(BlueprintType)
struct FModioEditModParams
{
    GENERATED_BODY()

public:

    /**
    * The name of the mod
    **/
    TOptional<FString> Name;
    
    /**
    * A brief summary of what the mod is
    **/
    TOptional<FString> Summary;
    
    /**
    * Optional override for the name 'slug' in the mod's URL
    **/
    TOptional<FString> NamePath;
    
    /**
    * Optional override for the mod's visibility status. Defaults to true (visible)
    * @deprecated Use Visibility property instead
	**/
	TOptional<bool> bVisible_DEPRECATED;
	
	/**
	* Enum parameter to signal the backend if the mod to upload would be publicly visible. Default 
    * value is Public
	**/
	TOptional<EModioObjectVisibilityFlags> Visibility;
    
    /**
    * Optional long description of the mod
    **/
    TOptional<FString> Description;
    
    /**
    * Optional URL to the mod's homepage. Must be a valid URL
    **/
    TOptional<FString> HomepageURL;
    
    /**
    * Bitwise mask of flags indicating the type of mature content present in this mod
    **/
    TOptional<EModioMaturityFlags> MaturityFlags;
    
    /**
    * Optional metadata blob for this mod
    **/
    TOptional<FString> MetadataBlob;

    /**
	 * Optional path to a new logo image for this mod
	 **/
	TOptional<FString> LogoPath;
};