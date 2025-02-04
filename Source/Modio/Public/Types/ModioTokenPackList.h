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

#include "Types/ModioList.h"
#include "Types/ModioTokenPack.h"
#include "Types/ModioPagedResult.h"

#include "ModioTokenPackList.generated.h"

#if CPP
struct MODIO_API FModioTokenPackList : public FModioPagedResult, public FModioList<TArray, FModioTokenPack>
{
	/** 
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioTokenPackList() = default;

	/** 
	 * @docpublic
	 * @brief Constructor that takes an array of token packs and an index of paged results
	 * @param PagedResult The paged result containing the pagination details
	 * @param TokenPackList The list of token packs
	 */
	FModioTokenPackList(const FModioPagedResult& PagedResult, TArray<FModioTokenPack>&& TokenPackList);

	/** 
	 * @docpublic
	 * @brief Constructor that takes only an array of token packs
	 * @param TokenPackList The list of token packs
	 */
	FModioTokenPackList(TArray<FModioTokenPack> TokenPackList);
};

#else

/** 
 * @docpublic
 * @brief Strong type struct to wrap multiple TokenPacks indexed by a paged result
 */
USTRUCT(NoExport, BlueprintType)
struct MODIO_API FModioTokenPackList
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief A paged result property
	 */
	UPROPERTY()
	FModioPagedResult PagedResult;

	/** 
	 * @docpublic
	 * @brief Array of Token Packs
	 */
	UPROPERTY()
	TArray<FModioTokenPack> InternalList;
};

#endif

/** 
 * @docpublic
 * @brief Struct to wrap ModInfoList into an optional parameter
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalTokenPackList
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioOptionalTokenPackList() = default;

	/** 
	 * @docpublic
	 * @brief Constructor with a Token Pack list parameter to initialize an instance
	 * @param TokenPackList Optional value of a FModioTokenPackList
	 */
	FModioOptionalTokenPackList(TOptional<FModioTokenPackList>&& TokenPackList);

	/** 
	 * @docpublic
	 * @brief Stored optional ModioTokenPackList
	 */
	TOptional<FModioTokenPackList> Internal;
};
