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

#include "Types/ModioCommonTypes.h"
#include "Types/ModioList.h"
#include "Types/ModioPagedResult.h"
#include "Types/ModioRating.h"

#include "ModioUserModRating.generated.h"

namespace Modio
{
	struct UserRatingList;
}


USTRUCT(BlueprintType)
struct MODIO_API FModioUserModRating
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|User Mod Rating");
	FModioGameID GameId;

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|User Mod Rating");
	FModioModID ModId;

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|User Mod Rating");
	EModioRating Rating;
};

#if CPP

struct MODIO_API FModioUserModRatingList : public FModioPagedResult, public FModioList<TArray, FModioUserModRating>
{
	FModioUserModRatingList() = default;
	FModioUserModRatingList(const struct Modio::UserRatingList& UserRatingList);
};
#else

/**
 * @docpublic
 * @brief Strong type struct to wrap multiple user ratings indexed by a paged result
 */
USTRUCT(NoExport, BlueprintType)
struct MODIO_API FModioUserModRatingList
{
	GENERATED_BODY();

	/**
	 * @docpublic
	 * @brief A paged result property
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UserModRatingList")
	FModioPagedResult PagedResult;

	/**
	 * @docpublic
	 * @brief Internal array of user ratings
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UserModRatingList")
	TArray<FModioUserModRating> InternalList;
};

#endif 

/**
 * @docpublic
 * @brief Struct to wrap UserModRatingList into an optional parameter
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalUserModRatingList
{
	GENERATED_BODY();

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioOptionalUserModRatingList() = default;

	/**
	 * @docpublic
	 * @brief Constructor with a user list parameter to initialize an instance
	 * @param UserModRatingList Optional value of a FModioUserModRatingList
	 */
	FModioOptionalUserModRatingList(TOptional<FModioUserModRatingList>&& UserModRatingList);

	/**
	 * @docpublic
	 * @brief Stored optional FModioUserModRatingList
	 */
	TOptional<FModioUserModRatingList> Internal;
};
