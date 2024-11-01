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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/DateTime.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioModInfo.h"

#include "ModioFilterParams.generated.h"

/// Forward declaration of SDK type for declaration of conversion function below
namespace Modio
{
	class FilterParams;
}

/// @brief Enum indicating which field should be used to sort the results
UENUM(BlueprintType)
enum class EModioSortFieldType : uint8
{
	ID, /** use mod ID (default) */
	DownloadsToday, /** use number of downloads in last 24 (exposed in REST API as 'popular' */
	SubscriberCount, /** use number of subscribers */
	Rating, /** use mod rating */
	DateMarkedLive, /** use date mod was marked live */
	DateUpdated, /** use date mod was last updated */
	DownloadsTotal, /** use downloads total */
	Alphabetical /** use mod name */
};

/// @brief Enum indicating which direction sorting should be applied
UENUM(BlueprintType)
enum class EModioSortDirection : uint8
{
	Ascending, /** (default) */
	Descending
};

/// @brief Enum indicating filtering options based off revenue type
UENUM(BlueprintType)
enum class EModioRevenueFilterType : uint8
{
	Free = 0, /** Return only free mods */
	Paid = 1, /** Return only paid mods */
	FreeAndPaid = 2 /** Return both free and paid mods */
};

/** @brief Class storing a set of filter parameters for use in xref:ListAllModsAsync[] */
USTRUCT(BlueprintType)
struct MODIO_API FModioFilterParams
{
	GENERATED_BODY()

	/**
	 * @brief Only include mods with the specified author User ID
	 * @param UserID The User ID to filter on
	 * @return *this
	 */
	FModioFilterParams& MatchingAuthor(const FModioUserID& UserID);

	/**
	 * @brief Only include mods with the specified author User IDs
	 * @param UserIDs The list of User IDs to filter on
	 * @return *this
	 */
	FModioFilterParams& MatchingAuthors(const TArray<FModioUserID>& UserIDs);

	/**
	 * @brief Indicates the filter should only include the specified mods
	 * @param IDs the set of mods to match
	 * @return *this
	 */
	FModioFilterParams& MatchingIDs(const TArray<FModioModID>& IDs);

	/**
	 * @brief Indicates the filter should exclude the specified mods.
	 * @param IDs the set of mods to exclude
	 * @return *this
	 */
	FModioFilterParams& ExcludingIDs(const TArray<FModioModID>& IDs);

	/**
	 * @brief Indicates results should be sorted using the specified field and direction
	 * @param ByField Field to sort with
	 * @param ByDirection Direction to sort
	 * @return *this
	 **/
	FModioFilterParams& SortBy(EModioSortFieldType ByField, EModioSortDirection ByDirection);

	/**
	 * @brief Only include mods where the name contains the provided string
	 * @param SearchString Search string
	 * @return *this
	 **/
	FModioFilterParams& NameContains(const FString& SearchString);

	/**
	 * @brief Only include mods where the name contains at least one of the provided strings (string1 OR string2 OR
	 *stringN...)
	 * @tparam ...Args std::string
	 * @param SearchString First search string
	 * @param ...args Additional search strings
	 * @return *this
	 **/
	FModioFilterParams& NameContains(const TArray<FString>& SearchString);

	/**
	 * @brief Only include mods that were marked live (i.e released) after the specified date
	 * @param LiveAfter Minimum date
	 * @return *this
	 **/
	FModioFilterParams& MarkedLiveAfter(FDateTime LiveAfter);

	/**
	 * @brief Only include mods that were marked live (i.e released) before the specified date
	 * @param LiveBefore Maximum date
	 * @return *this
	 **/
	FModioFilterParams& MarkedLiveBefore(FDateTime LiveBefore);

	/**
	 * @brief Only include mods with a metadata blob containing the specified substring
	 * @param SearchString The substring to search for
	 * @return *this
	 **/
	FModioFilterParams& MetadataLike(FString SearchString);

	/**
	 * @brief Only include mods that have the specified tag
	 * @param Tag Tag to include
	 * @return *this
	 **/
	FModioFilterParams& WithTags(const FString& Tag);

	/**
	 * @brief Only include mods that have all the specified tags (tag1 AND tag2 AND tagN...)
	 * @param NewTags The set of tags to filter on
	 * @return *this
	 **/
	FModioFilterParams& WithTags(const TArray<FString>& NewTags);

	/**
	 * @brief Only include mods that do not have the specified tag
	 * @param Tag Tag to exclude
	 * @return *this
	 **/
	FModioFilterParams& WithoutTags(const FString& Tag);

	/**
	 * @brief Only include mods that do not have any of the specified tags ( NOT (tag1 OR tag2 OR tagN...))
	 * @param NewTags Tags to exclude
	 * @return *this
	 **/
	FModioFilterParams& WithoutTags(const TArray<FString>& NewTags);

	/**
	 * @brief Returns a sub-range of query results from StartIndex to StartIndex + ResultCount
	 * @param StartIndex Zero-based index of first result to return
	 * @param ResultCount Number of results to return
	 * @return *this
	 **/
	FModioFilterParams& IndexedResults(uint64 StartIndex, uint64 ResultCount);

	/**
	 * @brief Returns a sub-range of query results based on a specified page size and index
	 * @param PageNumber Zero-based index of page to return
	 * @param PageSize Number of results in a page
	 * @return
	 **/
	FModioFilterParams& PagedResults(uint64 PageNumber, uint64 PageSize);

	/**
	 * @brief Returned mods according to the specified revenue type (free, paid, both)
	 * @param RevenueFilter Filter to use
	 * @return *this
	 */
	FModioFilterParams& RevenueType(EModioRevenueFilterType RevenueFilter);

	/**
	 * @brief Indicates results should exclude all mods which contain mature content
	 * @return *this
	 */
	FModioFilterParams& DisallowMatureContent();

	/**
	 * @brief Indicates results should be filtered by maturity options
	 * @param ByMaturity Maturity flags to filter by
	 * @return *this
	 */
	FModioFilterParams& WithMatureContentFlags(EModioMaturityFlags ByMaturity);

	/**
	 *  @brief Converts the filter params to a string suitable for use in the REST API.
	 *  @note Performs a allocation to acquire the string
	 *  @return FString containing the filter parameters
	 */
	FString ToString() const;

public:
	friend class Modio::FilterParams ToModio(const FModioFilterParams& In);

	EModioSortDirection Direction = EModioSortDirection::Ascending;
	EModioSortFieldType SortField = EModioSortFieldType::ID;
	TArray<FString> SearchKeywords;
	TOptional<FDateTime> DateRangeBegin;
	TOptional<FDateTime> DateRangeEnd;
	TArray<FString> Tags;
	TArray<FString> ExcludedTags;
	TArray<FModioUserID> AuthorUserIds;
	TArray<FModioModID> IncludedIDs;
	TArray<FModioModID> ExcludedIDs;
	TOptional<FString> MetadataBlobSearchString;
	TOptional<EModioRevenueFilterType> Revenue;
	TOptional<EModioMaturityFlags> Maturity;
	bool isPaged = false;
	int64 Index = 0;
	int64 Count = 100;
};

/**
 * Helper struct for named preset filter parameters
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioPresetFilterParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	FText PresetName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	TArray<FString> Tags;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	TArray<FString> ExcludedTags;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	EModioSortDirection Direction = EModioSortDirection::Ascending;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	EModioSortFieldType SortField = EModioSortFieldType::ID;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	int64 Count = 20;

	FModioFilterParams ToFilterParams() const
	{
		return FModioFilterParams()
			.WithTags(Tags)
			.WithoutTags(ExcludedTags)
			.SortBy(SortField, Direction)
			.IndexedResults(0, Count);
	}
};

UCLASS()
class MODIO_API UModioPresetFilterParamsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

protected:
	// Compiles a preset filter into a concrete set of filter parameters that can be passed to the mod.io plugin
	UFUNCTION(BlueprintCallable, Category = "mod.io|Filter Params")
	static FModioFilterParams ToFilterParams(const FModioPresetFilterParams& Preset)
	{
		return Preset.ToFilterParams();
	}
};