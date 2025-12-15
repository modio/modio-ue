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

#include "Misc/DateTime.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioGamePlatform.h"
#include "Types/ModioGameStats.h"
#include "Types/ModioHeaderImage.h"
#include "Types/ModioIcon.h"
#include "Types/ModioLogo.h"
#include "Types/ModioModTagOptions.h"
#include "Types/ModioOtherUrl.h"
#include "Types/ModioTheme.h"

#include "ModioGameInfo.generated.h"

namespace Modio
{
	struct GameInfo;
}

/*
 * @docpublic
 * @brief Cloud Cooking status for this game
 */
UENUM(BlueprintType)
enum class EModioGameCloudCookingStatus : uint8
{
	Disabled = 0,
	Initializing = 1,
	Initialized = 2,
	Finalizing = 3,
	Enabled = 4,
	ActionRequired = 5,
	TearingDown = 6
};

/**
 * @docpublic
 * @brief Monetization properties of a game
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EGameMonetizationFlags : uint8
{
	// None set (default)
	None = 0,
	// Monetization is enabled
	Monetization = 1,
	// Marketplace is enabled
	Marketplace = 2,
	// Partner Program is enabled
	PartnerProgram = 4,
};

/**
 * @docpublic
 * @brief Maturity options for a game
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EGameMaturityFlags : uint8
{
	// Don't allow mature content in mods (default)
	None = 0,
	// This game allows mods containing mature content
	MatureModsAllowed = 1,
	// This game is for mature audiences only
	MatureAudiencesOnly = 2,
};

/**
 * @docpublic
 * @brief Full game profile with extended information
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioGameInfo
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Unique Game ID
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioGameID GameID;

	/**
	 * @docpublic
	 * @brief Unix timestamp of date the game was registered
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FDateTime DateAdded;

	/**
	 * @docpublic
	 * @brief Unix timestamp of date the game was updated
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FDateTime DateUpdated;

	/**
	 * @docpublic
	 * @brief Unix timestamp of date the game was set live
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FDateTime DateLive;

	/**
	 * @docpublic
	 * @brief Word used to describe user-generated content (mods, items, add-ons etc)
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString UgcName;

	/**
	 * @docpublic
	 * @brief Contains media URLs to the icon for the game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioIcon Icon = {};

	/**
	 * @docpublic
	 * @brief Contains media URLs to the logo for the game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioLogo Logo = {};

	/**
	 * @docpublic
	 * @brief Contains media URLs to the preview header image for the game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioHeaderImage HeaderImage = {};

	/**
	 * @docpublic
	 * @brief Name of the game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString Name;

	/**
	 * @docpublic
	 * @brief Summary of the game's mod support
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString Summary;

	/**
	 * @docpublic
	 * @brief A guide about creating and uploading mods for this game to mod.io
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString Instructions;

	/**
	 * @docpublic
	 * @brief Link to a mod.io guide, modding wiki, or a page where modders can learn how to make and submit mods to
	 * this game's profile
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString InstructionsUrl;

	/**
	 * @docpublic
	 * @brief URL to the game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString ProfileUrl;

	/**
	 * @docpublic
	 * @brief Theme color values for the game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioTheme Theme = {};

	/**
	 * @docpublic
	 * @brief Numerous aggregate stats for the game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FModioGameStats Stats;

	/**
	 * @docpublic
	 * @brief Creator defined URLs to share
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	TArray<FModioOtherUrl> OtherUrls = {};

	/**
	 * @docpublic
	 * @brief Platforms that are supported by this title
	 */
	UPROPERTY(
		meta = (DeprecatedProperty,
				DeprecationMessage = "Deprecated as of 2023.9 release. Please use the <<GamePlatforms>> instead."),
		BlueprintReadOnly, Category = "mod.io|GameInfo")
	TArray<EModioModfilePlatform> Platforms = {};

	/**
	 * @docpublic
	 * @brief Whether or not the game allows negative ratings
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	bool bAllowNegativeRatings = false;

	/**
	 * @docpublic
	 * @brief Monetization options for the game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	EGameMonetizationFlags GameMonetizationOptions = EGameMonetizationFlags::None;

	/**
	 * @docpublic
	 * @brief Maturity options for the game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	EGameMaturityFlags GameMaturityOptions = EGameMaturityFlags::None;

	/**
	 * @docpublic
	 * @brief Name of the Virtual Tokens for this game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	FString VirtualTokenName = TEXT("");

	/**
	 * @docpublic
	 * @brief Platforms that are supported by this title
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	TArray<FModioGamePlatform> PlatformSupport = {};

	/**
	 * @docpublic
	 * @brief Tags available for this game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	TArray<FModioModTagInfo> TagOptions;
	/**
	 * @docpublic
	 * @brief Cloud Cooking status for this game
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|GameInfo")
	EModioGameCloudCookingStatus CloudCookingStatus = EModioGameCloudCookingStatus::Disabled;

	friend struct FModioGameInfo ToUnreal(const struct Modio::GameInfo& In);
};

/**
 * @docpublic
 * @brief Strong type struct to store an optional GameInfo parameter
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalGameInfo
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Stored property to an optional GameInfo
	 */
	TOptional<FModioGameInfo> Internal;
};
