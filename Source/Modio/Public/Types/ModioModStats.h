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

#include "Containers/UnrealString.h"

#include "ModioModStats.generated.h"

/** @brief Contains download stats and ratings for a mod */
USTRUCT(BlueprintType)
struct MODIO_API FModioModStats
{
	GENERATED_BODY()

	/** @brief Current rank of the mod. */
	UPROPERTY(BlueprintReadOnly, Category = "ModStats|Popularity")
	int64 PopularityRankPosition;

	/** @brief Number of ranking spots the current rank is measured against.  */
	UPROPERTY(BlueprintReadOnly, Category = "ModStats|Popularity")
	int64 PopularityRankTotalMods;

	/** @brief Number of total mod downloads.  */
	UPROPERTY(BlueprintReadOnly, Category = "ModStats")
	int64 DownloadsTotal;

	/** @brief Number of total users who have subscribed to the mod.  */
	UPROPERTY(BlueprintReadOnly, Category = "ModStats")
	int64 SubscribersTotal;

	/** @brief Number of times this mod has been rated. */
	UPROPERTY(BlueprintReadOnly, Category = "ModStats|Rating")
	int64 RatingTotal;

	/** @brief Number of positive ratings. */
	UPROPERTY(BlueprintReadOnly, Category = "ModStats|Rating")
	int64 RatingPositive;

	/** @brief Number of negative ratings. */
	UPROPERTY(BlueprintReadOnly, Category = "ModStats|Rating")
	int64 RatingNegative;

	/** @brief Number of positive ratings, divided by the total ratings to determine it’s percentage score. */
	UPROPERTY(BlueprintReadOnly, Category = "ModStats|Rating")
	int64 RatingPercentagePositive;

	/**
	 * @brief Overall rating of this item calculated using the [Wilson score confidence
	 * interval](https://www.evanmiller.org/how-not-to-sort-by-average-Ratinghtml). This column is good to sort
	 * on, as it will order items based on number of ratings and will place items with many positive ratings
	 * above those with a higher score but fewer ratings.
	 * We actually get a double back from the server, but it's converted to a float for blueprint support
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "ModStats|Rating")
	float RatingWeightedAggregate;

	/**
	 * @brief Textual representation of the rating in format: Overwhelmingly
	 * Positive -> Very Positive -> Positive -> Mostly Positive -> Mixed ->
	 * Negative -> Mostly Negative -> Very Negative -> Overwhelmingly
	 * Negative -> Unrated
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "ModStats|Rating")
	FString RatingDisplayText;
};
