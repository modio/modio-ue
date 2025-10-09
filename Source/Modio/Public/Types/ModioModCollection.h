/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "ModioModInfo.h"
#include "Misc/Optional.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioUser.h"
#include "Types/ModioList.h"
#include "Types/ModioLogo.h"
#include "Types/ModioPagedResult.h"

#include "ModioModCollection.generated.h"

namespace Modio
{
	struct ModCollectionInfo;
	class ModCollectionInfoList;
}

/**
 * @docpublic
 * @brief Contains download stats and ratings for a mod collection
*/
USTRUCT(BlueprintType)
struct MODIO_API FModioModCollectionStats
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief Number of mod collection downloads today.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 DownloadsToday = 0;

	/** 
	 * @docpublic
	 * @brief Number of total mod collection downloads.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 DownloadsTotal = 0;

	/** 
	 * @docpublic
	 * @brief Number of unique mod collection downloads.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 DownloadsUnique = 0;

	/** 
	 * @docpublic
	 * @brief Number of rating over 30 days.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 RatingTotal30Days = 0;

	/** 
	 * @docpublic
	 * @brief Number of positive ratings over 30 days.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 RatingPositive30Days = 0;

	/** 
	 * @docpublic
	 * @brief Number of negative ratings over 30 days.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 RatingNegative30Days = 0;

	/** 
	 * @docpublic
	 * @brief Number of times this mod collection has been rated.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 RatingTotal = 0;

	/** 
	 * @docpublic
	 * @brief Number of positive ratings.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 RatingPositive = 0;

	/** 
	 * @docpublic
	 * @brief Number of negative ratings.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 RatingNegative = 0;

	/** 
	 * @docpublic
	 * @brief Number of comments.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 NumberOfComments = 0;

	/** 
	 * @docpublic
	 * @brief Number of followers.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 NumberOfFollowers = 0;

	/** 
	 * @docpublic
	 * @brief Number of mods in the collection.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionStats")
	int64 NumberOfMods = 0;
};

/**
 * @docpublic
 * @brief Information about a given mod collection
*/
USTRUCT(BlueprintType)
struct MODIO_API FModioModCollectionInfo
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief The unique ID of the collection.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FModioModCollectionID Id;

	/** 
	 * @docpublic
	 * @brief The unique ID of the game the collection belongs to.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FModioGameID GameId;

	/** 
	 * @docpublic
	 * @brief the game name
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FString GameName;

	/** 
	 * @docpublic
	 * @brief The current ModStatus on the server: Accepted, NotAccepted, or Deleted.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	EModioModServerSideStatus CollectionStatus = EModioModServerSideStatus::NotAccepted;

	/** 
	 * @docpublic
	 * @brief The mod collection's visibility status. Defaults to Public (1)
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	EModioObjectVisibilityFlags Visibility = EModioObjectVisibilityFlags::Public;

	/** 
	 * @docpublic
	 * @brief Information on the user who submitted the collection
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FModioUser ProfileSubmittedBy;

	/** 
	 * @docpublic
	 * @brief Chosen category of the collection
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FString Category;

	/** 
	 * @docpublic
	 * @brief Unix timestamp of the date the mod was registered
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FDateTime ProfileDateAdded;

	/** 
	 * @docpublic
	 * @brief Unix timestamp of the date the mod was updated
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FDateTime ProfileDateUpdated;

	/** 
	 * @docpublic
	 * @brief Unix timestamp of the date the mod was marked live
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FDateTime ProfileDateLive;

	/** 
	 * @docpublic
	 * @brief boolean to indicate whether the collection contains hidden mods
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	bool Incomplete = false;

	/** 
	 * @docpublic
	 * @brief Maturity options flagged by the mod developer.
	 * This is only relevant if the parent game allows collections to be labeled as mature.
	 * The value of this field will default to None unless the parent game allows to flag mature content.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	EModioMaturityFlags ProfileMaturityOption{};

	/** 
	 * @docpublic
	 * @brief Size of collection in bytes 
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FModioUnsigned64 FileSize;

	/** 
	 * @docpublic
	 * @brief Size of the uncompressed collection in bytes
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FModioUnsigned64 FileSizeUncompressed;

	/** 
	 * @docpublic
	 * @brief Supported platform for this collection
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	TArray<EModioModfilePlatform> Platforms;

	/** 
	 * @docpublic
	 * @brief Tags this mod collection has set
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	TArray<FString> Tags;

	/** 
	 * @docpublic
	 * @brief Stats and rating information for the mod collection.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FModioModCollectionStats Stats;

	/** 
	 * @docpublic
	 * @brief Media data related to the mod collection logo
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FModioLogo Logo;

	/** 
	 * @docpublic
	 * @brief Name of the mod collection
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FString ProfileName;

	/** 
	 * @docpublic
	 * @brief Name id of the mod collection
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FString ProfileNameId;

	/** 
	 * @docpublic
	 * @brief Summary of the mod collection
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FString ProfileSummary;

	/** 
	 * @docpublic
	 * @brief Detailed description in HTML format
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ModCollectionInfo")
	FString ProfileDescription;

	friend struct FModioModCollectionInfo ToUnreal(const struct Modio::ModCollectionInfo& In);

	bool operator==(const FModioModCollectionInfo& Other) const
	{
		return Other.Id == Id;
	}

	bool operator==(const FModioModCollectionID& Other) const
	{
		return Other == Id;
	}

	friend uint32 GetTypeHash(const FModioModCollectionInfo& Info)
	{
		return FCrc::MemCrc32(&Info.Id, sizeof(FModioModCollectionID));
	}
};

#if CPP
/**
 * @docpublic
 * @brief Struct to bring forward a native version of FModioModCollectionInfoList
 */
struct MODIO_API FModioModCollectionInfoList : FModioPagedResult, FModioList<TArray, FModioModCollectionInfo>
{
	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioModCollectionInfoList() = default;

	/**
	 * @docpublic
	 * @brief Constructor that takes an array of mod collection info and an index of paged results
	 */
	FModioModCollectionInfoList(const FModioPagedResult& PagedResult,
	                            TArray<FModioModCollectionInfo>&& ModCollectionInfoList);

	/**
	 * @docpublic
	 * @brief Constructor that takes a mod collection info list to store in this struct
	 */
	FModioModCollectionInfoList(const class Modio::ModCollectionInfoList& ModCollectionInfoList);
};

#else
/** 
 * @docpublic
 * @brief Strong type struct to wrap multiple ModCollectionInfo indexed by a paged result 
 */
USTRUCT(NoExport, BlueprintType)
struct MODIO_API FModioModCollectionInfoList
{
	/**
	 * @docpublic
	 * @brief A paged result property
	 */
	UPROPERTY()
	FModioPagedResult PagedResult;

	/**
	 * @docpublic
	 * @brief Array of mod collection info
	 */
	UPROPERTY()
	TArray<FModioModCollectionInfo> InternalList;
};

#endif

/**
 * @docpublic
 * @brief Struct to wrap ModCollectionInfoList into an optional parameter
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModCollectionInfoList
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioOptionalModCollectionInfoList() = default;

	/**
	 * @docpublic
	 * @brief Constructor with a mod collection info list parameter to initialize an instance
	 * @param ModCollectionInfoList Optional value of a FModioModCollectionInfoList
	 */
	FModioOptionalModCollectionInfoList(TOptional<FModioModCollectionInfoList>&& ModCollectionInfoList);

	/**
	 * @docpublic
	 * @brief Stored optional ModioModCollectionInfoList
	 */
	TOptional<FModioModCollectionInfoList> Internal;
};

/**
 * @docpublic
 * @brief Struct to wrap ModCollectionInfo into an optional parameter
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModCollectionInfo
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Stored optional ModioModCollectionInfo
	 */
	TOptional<FModioModCollectionInfo> Internal;
};