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
 * @docpublic
 * @brief Struct to store links and metadata associated to YouTube URLs
 */
struct MODIO_API FModioYoutubeURLList : public FModioList<TArray, FString>
{
	/** 
	 * @docpublic
	 * @brief ModioYoutubeURLList default constructor
	 */
	FModioYoutubeURLList() = default;

	/** 
	 * @docpublic
	 * @brief ModioYoutubeURLList constructor based on a Modio::YoutubeURLList
	 * @param URLList Elements to copy over this struct
	 */
	FModioYoutubeURLList(const Modio::YoutubeURLList& URLList);
};

/**
 * @docpublic
 * @brief Struct to store links and metadata associated to Sketchfab URLs
 */
struct MODIO_API FModioSketchfabURLList : public FModioList<TArray, FString>
{
	/** 
	 * @docpublic
	 * @brief ModioSketchfabURLList default constructor
	 */
	FModioSketchfabURLList() = default;

	/** 
	 * @docpublic
	 * @brief ModioSketchfabURLList constructor based on a Modio::SketchfabURLList
	 * @param URLList Elements to copy over this struct
	 */
	FModioSketchfabURLList(const Modio::SketchfabURLList& URLList);
};
#else
/**
 * @docpublic
 * @brief Provides blueprint support to an array of strings
 */
struct FModioURLListInternal
{
	/** 
	 * @docpublic
	 * @brief Internal list of strings for storing URLs
	 */
	TArray<FString> InternalList;
};

/**
 * @docpublic
 * @brief Struct to store links and metadata associated to YouTube URLs for Blueprint
 */
USTRUCT(NoExport, BlueprintType)
struct FModioYoutubeURLList
{
	/** 
	 * @docpublic
	 * @brief Internal list of YouTube URLs
	 */
	FModioURLListInternal InternalList;
};

/**
 * @docpublic
 * @brief Struct to store links and metadata associated to Sketchfab URLs for Blueprint
 */
USTRUCT(NoExport, BlueprintType)
struct FModioSketchfabURLList
{
	/** 
	 * @docpublic
	 * @brief Internal list of Sketchfab URLs
	 */
	FModioURLListInternal InternalList;
};
#endif
