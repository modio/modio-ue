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
 * Struct to bring forward a native version of FModioGameInfoList
 **/
struct MODIO_API FModioGameInfoList : public FModioPagedResult, public FModioList<TArray, FModioGameInfo>
{

	/**
	 * Default constructor without parameters
	 **/
	FModioGameInfoList() = default;

	/**
	 * Constructor that takes an array game info and a index of paged results
	 **/
	FModioGameInfoList(const FModioPagedResult& PagedResult, TArray<FModioGameInfo>&& GameInfoList);

	/**
	 * Constructor that takes a game info list to store in this struct
	 **/
	FModioGameInfoList(const class Modio::GameInfoList& GameInfoList);
};

#else
/**
 * Strong type struct to wrap multiple GameInfo indexed by a paged result
 **/
USTRUCT(NoExport, BlueprintType)
struct MODIO_API FModioGameInfoList
{
	/**
	 * A paged result property
	 **/
	UPROPERTY()
	FModioPagedResult PagedResult;

	/**
	 * Arrray of game info
	 **/
	UPROPERTY()
	TArray<FModioGameInfo> InternalList;
};

#endif

/**
 * Struct to wrap GameInfoList into an optional parameter
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalGameInfoList
{
	GENERATED_BODY()

	/**
	 * Default constructor without parameters
	 **/
	FModioOptionalGameInfoList() = default;

	/**
	 * Constructor with a game info list parameter to initialize an instance
	 * @param GameInfoList Optional value of a FModioModInfoList
	 **/
	FModioOptionalGameInfoList(TOptional<FModioGameInfoList>&& GameInfoList);

	/**
	 * Stored optional ModioGameInfoList
	 **/
	TOptional<FModioGameInfoList> Internal;
};
