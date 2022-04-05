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

#include "Types/ModioCommonTypes.h"

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
	DateUpdated /** use date mod was last updated */
};

/// @brief Enum indicating which direction sorting should be applied
UENUM(BlueprintType)
enum class EModioSortDirection : uint8
{
	Ascending, /** (default) */
	Descending
};

/** @brief Class storing a set of filter parameters for use in xref:ListAllModsAsync[] */
USTRUCT(BlueprintType)
struct MODIO_API FModioFilterParams
{
	GENERATED_BODY()

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
	 *  @brief Converts the filter params to a string suitable for use in the REST API.
	 *  @note Performs a allocation to acquire the string
	 *  @return FString containing the filter parameters
	 */
	FString ToString() const;

private:
	
	friend class Modio::FilterParams ToModio(const FModioFilterParams& In );
	friend class UModioSearchResultsView;

	EModioSortDirection Direction = EModioSortDirection::Ascending;
	EModioSortFieldType SortField = EModioSortFieldType::ID;
	TArray<FString> SearchKeywords;
	TOptional<FDateTime> DateRangeBegin;
	TOptional<FDateTime> DateRangeEnd;
	TArray<FString> Tags;
	TArray<FString> ExcludedTags;
	TArray<FModioModID> IncludedIDs;
	TArray<FModioModID> ExcludedIDs;
	TOptional<FString> MetadataBlobSearchString;
	bool isPaged = false;
	int64 Index = 0;
	int64 Count = 100;
};
