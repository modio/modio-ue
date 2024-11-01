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
#include "Types/ModioList.h"

#include "ModioURLList.generated.h"

namespace Modio
{
	class YoutubeURLList;
	class SketchfabURLList;
}

#if CPP
/**
* Struct to store links and metadata associated to youtube URLs
**/
struct MODIO_API FModioYoutubeURLList : public FModioList<TArray, FString>
{
	/**
	* ModioYoutubeURLList default constructor
	**/
	FModioYoutubeURLList() = default;

	/**
	* ModioYoutubeURLList constructor based on a Modio::YoutubeURLList
	* @param URLList Elements to copy over this struct
	**/
	FModioYoutubeURLList(const Modio::YoutubeURLList& URLList);
};

/**
* Struct to store links and metadata associated to sketchfab URLs
**/
struct MODIO_API FModioSketchfabURLList : public FModioList<TArray, FString>
{
	/**
	* ModioSketchfabURLList default constructor
	**/
	FModioSketchfabURLList() = default;
	
	/**
	* ModioSketchfabURLList constructor based on a Modio::SketchfabURLList
	* @param URLList Elements to copy over this struct
	**/
	FModioSketchfabURLList(const Modio::SketchfabURLList& URLList);
};
#else
/**
* Provides blueprint support to an array of strings
**/
struct FModioURLListInternal
{
	TArray<FString> InternalList;
};

/**
* Struct to store links and metadata associated to youtube URLs
**/
USTRUCT(NoExport, BlueprintType)
struct FModioYoutubeURLList
{
	FModioURLListInternal InternalList;
};

/**
* Struct to store links and metadata associated to sketchfab URLs
**/
USTRUCT(NoExport, BlueprintType)
struct FModioSketchfabURLList
{
	FModioURLListInternal InternalList;
};
#endif
