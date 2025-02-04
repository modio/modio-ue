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
#include "Types/ModioGameInfo.h"
#include "Types/ModioPagedResult.h"

#include "ModioGameInfoList.generated.h"

namespace Modio
{
	class GameInfoList;
}

#if CPP
/**
 * @docpublic
 * @brief Struct to bring forward a native version of FModioGameInfoList
 */
struct MODIO_API FModioGameInfoList : public FModioPagedResult, public FModioList<TArray, FModioGameInfo>
{
	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioGameInfoList() = default;

	/**
	 * @docpublic
	 * @brief Constructor that takes an array of game info and an index of paged results
	 * @param PagedResult A paged result object
	 * @param GameInfoList A list of game info to store
	 */
	FModioGameInfoList(const FModioPagedResult& PagedResult, TArray<FModioGameInfo>&& GameInfoList);

	/**
	 * @docpublic
	 * @brief Constructor that takes a game info list to store in this struct
	 * @param GameInfoList A Modio::GameInfoList object to wrap
	 */
	FModioGameInfoList(const class Modio::GameInfoList& GameInfoList);
};

#else
/**
 * @docpublic
 * @brief Strong type struct to wrap multiple GameInfo indexed by a paged result
 */
USTRUCT(NoExport, BlueprintType)
struct MODIO_API FModioGameInfoList
{
	GENERATED_BODY()

	/**
	 * @brief A paged result property
	 */
	UPROPERTY()
	FModioPagedResult PagedResult;

	/**
	 * @brief Array of game info
	 */
	UPROPERTY()
	TArray<FModioGameInfo> InternalList;
};

#endif

/**
 * @docpublic
 * @brief Struct to wrap GameInfoList into an optional parameter
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalGameInfoList
{
	GENERATED_BODY()

	/**
	 * @brief Default constructor without parameters
	 */
	FModioOptionalGameInfoList() = default;

	/**
	 * @docpublic
	 * @brief Constructor with a game info list parameter to initialize an instance
	 * @param GameInfoList An optional value of a FModioGameInfoList
	 */
	FModioOptionalGameInfoList(TOptional<FModioGameInfoList>&& GameInfoList);

	/**
	 * @brief Stored optional ModioGameInfoList
	 */
	TOptional<FModioGameInfoList> Internal;
};