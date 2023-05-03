/*
 *  Copyright (C) 2021-2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Types/ModioCommonTypes.h"
#include "Types/ModioGameStats.h"
#include "Types/ModioHeaderImage.h"
#include "Types/ModioIcon.h"
#include "Types/ModioLogo.h"
#include "Types/ModioOtherUrl.h"
#include "Types/ModioTheme.h"

#include "ModioGameInfo.generated.h"

namespace Modio
{
	struct GameInfo;
}

/**
 * Full game profile with extended information
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioGameInfo
{
	GENERATED_BODY()

	/** @brief Unique Game ID */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioGameID GameID;

	/** @brief Unix timestamp of date the game was registered */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FDateTime DateAdded;

	/** @brief Unix timestamp of date the game was updated */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FDateTime DateUpdated;

	/** @brief Unix timestamp of date the game was set live */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FDateTime DateLive;

	/** @brief Word used to describe user-generated content (mods, items, add-ons etc) */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString UgcName;

	/** @brief Contains media URLs to the icon for the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioIcon Icon = {};

	/** @brief Contains media URLs to the logo for the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioLogo Logo = {};

	/** @brief Contains media URLs to the preview header image for the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioHeaderImage HeaderImage = {};

	/** @brief Name of the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString Name;

	/** @brief Summary of the game's mod support */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString Summary;

	/** @brief A guide about creating and uploading mods for this game to mod.io */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString Instructions;

	/** @brief Link to a mod.io guide, modding wiki, or a page where modders can learn how to make and submit mods */
	/// to this game's profile
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString InstructionsUrl;

	/** @brief URL to the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString ProfileUrl;

	/** @brief Theme color values for the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioTheme Theme = {};

	///** @brief Numerous aggregate stats for the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioGameStats Stats;

	///** @brief Creator defined URLs to share */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	TArray<FModioOtherUrl> OtherUrls = {};

	friend struct FModioGameInfo ToUnreal(const struct Modio::GameInfo& In);
};

/**
 * Strong type struct to store an optional GameInfo parameter
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalGameInfo
{
	GENERATED_BODY()
	/**
	 * Stored property to an optional GameInfo
	 **/
	TOptional<FModioGameInfo> Internal;
};
