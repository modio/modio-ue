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

#include "ModioGameStats.generated.h"

/** @brief Numerous aggregate stats for the game */
USTRUCT(BlueprintType)
struct MODIO_API FModioGameStats
{
	GENERATED_BODY()

	/** @brief Unique game id */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameStats")
	FModioGameID GameID = FModioGameID::InvalidGameID();
	/** @brief Available mod count for the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameStats")
	int64 ModCountTotal {};
	/** @brief Mods downloaded today for the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameStats")
	int64 ModDownloadsToday {};
	/** @brief Total mods downloaded for the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameStats")
	int64 ModDownloadsTotal {};
	/** @brief Average mods downloaded on a daily basis */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameStats")
	int64 ModDownloadsDailyAverage {};
	/** @brief Number of total users who have subscribed to the mods for the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameStats")
	int64 ModSubscribersTotal {};
	/** @brief Unix timestamp until this game's statistics are considered stale */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameStats")
	int64 DateExpires {};
};
