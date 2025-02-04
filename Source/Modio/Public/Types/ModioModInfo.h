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

#include "Types/ModioFileMetadata.h"
#include "Types/ModioMetadata.h"
#include "Types/ModioModStats.h"
#include "Types/ModioModTag.h"
#include "Types/ModioURLList.h"
#include "Types/ModioUnsigned64.h"
#include "Types/ModioUser.h"

#include "ModioModInfo.generated.h"

namespace Modio
{
	struct ModInfo;
}

/**
 * @docpublic
 * @brief Enum representing whether or not a mod is visible to users
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioObjectVisibilityFlags : uint8
{
	/** Mod is concealed from users */
	Hidden = 0,
	/** Mod is openly available */
	Public = 1
};

/**
 * @docpublic
 * @brief Enum representing mature content that a mod may contain
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioMaturityFlags : uint8
{
	// No maturity
	None,
	// Content contains alcohol references
	Alcohol = 1,
	// Content contains drug references
	Drugs = 2,
	// Content contains violence references
	Violence = 4,
	// Content contains sexual references
	Explicit = 8
};
ENUM_CLASS_FLAGS(EModioMaturityFlags);

/**
 * @brief Enum representing community options for a mod
 * Unreal's Enum class flags don't support uint32, so we use uint8 for bitmask handling
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioModCommunityOptionsFlags : uint8
{
	// No community options
	None = 0,
	// Comments are enabled
	EnableComments = 1,
	// Enable Previews
	EnablePreviews = 2,
	// Enable Preview URLs
	EnablePreviewURLs = 4,
	// Allow mod dependencies
	AllowDependencies = 8
};
ENUM_CLASS_FLAGS(EModioModCommunityOptionsFlags);

/**
 * @docpublic
 * @brief Enum representing a mod's server-side status
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioModServerSideStatus : uint8
{
	NotAccepted = 0,
	Accepted = 1,
	Deleted = 3
};

/**
 * @docpublic
 * @brief Full mod profile including current release information, media links, and stats
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModInfo
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief Unique Mod ID
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModInfo")
	FModioModID ModId;

	/** 
	 * @docpublic
	 * @brief Name of the mod
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	FString ProfileName;

	/** 
	 * @docpublic
	 * @brief Summary of the mod
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	FString ProfileSummary;

	/** 
	 * @docpublic
	 * @brief Detailed description in HTML format
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	FString ProfileDescription;

	/** 
	 * @docpublic
	 * @brief Detailed description in plaintext
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	FString ProfileDescriptionPlaintext;

	/** 
	 * @docpublic
	 * @brief URL to the mod profile
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	FString ProfileURL;

	/** 
	 * @docpublic
	 * @brief Information on the user who submitted the mod
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	FModioUser ProfileSubmittedBy;

	/** 
	 * @docpublic
	 * @brief Unix timestamp of the date the mod was registered
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	FDateTime ProfileDateAdded;

	/** 
	 * @docpublic
	 * @brief Unix timestamp of the date the mod was updated
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	FDateTime ProfileDateUpdated;

	/** 
	 * @docpublic
	 * @brief Unix timestamp of the date the mod was marked live
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	FDateTime ProfileDateLive;

	/**
	 * @docpublic
	 * @brief Flags for maturity options. Maturity options are flagged by the mod developer. This is only relevant if
	 * the parent game allows mods to be labeled as mature.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	EModioMaturityFlags ProfileMaturityOption {};

	/**
	 * @docpublic
	 * @brief Community options for the mod
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	EModioModCommunityOptionsFlags CommunityOptions {};

	/** 
	 * @docpublic
	 * @brief Is the mod marked as visible?
	 * @deprecated Use Visibility property instead
	 */
	UPROPERTY(
		meta = (DeprecatedProperty,
				DeprecationMessage =
					"Deprecated as of 2023.6 release. Please use `EModioObjectVisibilityFlags Visibility` instead."))
	bool bVisible_DEPRECATED {};

	/**
	 * @docpublic
	 * @brief Enum parameter to signal the backend if the mod to upload would be publicly visible.
	 * Default value is Public
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	EModioObjectVisibilityFlags Visibility {};

	/** 
	 * @docpublic
	 * @brief If this mod has any dependencies
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Profile")
	bool Dependencies = false;

	/**
	 * @docpublic
	 * @brief Stored property to the metadata string
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Metadata")
	FString MetadataBlob;

	/** 
	 * @docpublic
	 * @brief Information about the mod's most recent public release
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "File")
	FModioFileMetadata FileInfo;

	/** 
	 * @docpublic
	 * @brief Arbitrary key-value metadata set for this mod
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Metadata")
	TArray<FModioMetadata> MetadataKvp;

	/** 
	 * @docpublic
	 * @brief Tags this mod has set
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Metadata")
	TArray<FModioModTag> Tags;

	/** 
	 * @docpublic
	 * @brief Number of images in the mod's media gallery
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Media")
	int32 NumGalleryImages {};

	/** 
	 * @docpublic
	 * @brief List of youtube links provided by the creator of the mod
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Media")
	FModioYoutubeURLList YoutubeURLs;

	/** 
	 * @docpublic
	 * @brief List of sketchfab links provided by the creator of the mod
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Media")
	FModioSketchfabURLList SketchfabURLs;

	/** 
	 * @docpublic
	 * @brief Stats and rating information for the mod
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Stats")
	FModioModStats Stats;

	/** 
	 * @docpublic
	 * @brief Status of the mod
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Status")
	EModioModServerSideStatus ModStatus {};

	/** 
	 * @docpublic
	 * @brief Price of this mod
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Monetization")
	FModioUnsigned64 Price = FModioUnsigned64(0);

	friend struct FModioModInfo ToUnreal(const struct Modio::ModInfo& In);
};

/**
 * @docpublic
 * @brief Strong type struct to store an optional ModInfo parameter
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModInfo
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Stored property to an optional ModInfo
	 */
	TOptional<FModioModInfo> Internal;
};
