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

#include "Types/ModioList.h"
#include "Types/ModioModTagInfo.h"
#include "Types/ModioPagedResult.h"

#include "ModioModTagOptions.generated.h"

namespace Modio
{
	class ModTagOptions;
}

#if CPP
/**
 * @docpublic
 * @brief Strong type struct to wrap multiple ModTagInfo indexed by a paged result
 */
struct MODIO_API FModioModTagOptions : public FModioPagedResult, public FModioList<TArray, FModioModTagInfo>
{
	/** 
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioModTagOptions() = default;
	
	/** 
	 * @docpublic
	 * @brief Copy constructor with a mod tag options parameter
	 * @param ModInfoList Value that has mod tag options
	 */
	FModioModTagOptions(const Modio::ModTagOptions& ModInfoList);
};

#else
/** 
 * @docpublic
 * @brief Strong type struct to wrap multiple ModTagInfo indexed by a paged result
 */
USTRUCT(NoExport, BlueprintType)
struct MODIO_API FModioModTagOptions
{
	/** 
	 * @docpublic
	 * @brief Stored property to a paged result that indexes the current list of tag info
	 */
	UPROPERTY()
	FModioPagedResult PagedResult;

	/** 
	 * @docpublic
	 * @brief List wrapping multiple mod tag info
	 */
	UPROPERTY()
	TArray<FModioModTagInfo> InternalList;
};

#endif

/** 
 * @docpublic
 * @brief Strong type struct that wraps mod tag options in an optional value
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModTagOptions
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioOptionalModTagOptions() = default;
	
	/** 
	 * @docpublic
	 * @brief Constructor with mod tag options parameter
	 * @param ModTagOptions An optional value that has mod tag options
	 */
	FModioOptionalModTagOptions(TOptional<FModioModTagOptions>&& ModTagOptions);

	/** 
	 * @docpublic
	 * @brief The stored property of mod tag options
	 */
	TOptional<FModioModTagOptions> Internal;
};