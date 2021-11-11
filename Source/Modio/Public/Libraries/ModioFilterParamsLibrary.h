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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioFilterParams.h"

#include "ModioFilterParamsLibrary.generated.h"

UCLASS()
class MODIO_API UModioFilterParamsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * @brief Indicates the filter should only include the specified mods
	 * @param IDs the set of mods to match
	 * @return *this
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& MatchingIDs(UPARAM(ref) FModioFilterParams& Filter, const TArray<FModioModID>& IDs)
	{
		Filter.MatchingIDs(IDs);
		return Filter;
	}

	/**
	 * @brief Indicates the filter should exclude the specified mods.
	 * @param IDs the set of mods to exclude
	 * @return *this
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& ExcludingIDs(UPARAM(ref) FModioFilterParams& Filter, const TArray<FModioModID>& IDs)
	{
		Filter.ExcludingIDs(IDs);
		return Filter;
	}

	/**
	 * @brief Indicates results should be sorted using the specified field and direction
	 * @param ByField Field to sort with
	 * @param ByDirection Direction to sort
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& SortBy(UPARAM(ref) FModioFilterParams& Filter, EModioSortFieldType ByField,
									  EModioSortDirection ByDirection);

	/**
	 * @brief Only include mods where the name contains the provided string
	 * @param SearchString Search string
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& NameContains(UPARAM(ref) FModioFilterParams& Filter, const FString& SearchString);

	/**
	 * @brief Only include mods where the name contains the provided strings
	 * @param SearchString Search string
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& NameContainsStrings(UPARAM(ref) FModioFilterParams& Filter, const FString& SearchString);

	/**
	 * @brief Only include mods that were marked live (i.e released) after the specified date
	 * @param LiveAfter Minimum date
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& MarkedLiveAfter(UPARAM(ref) FModioFilterParams& Filter, FDateTime LiveAfter);

	/**
	 * @brief Only include mods that were marked live (i.e released) before the specified date
	 * @param LiveBefore Maximum date
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& MarkedLiveBefore(UPARAM(ref) FModioFilterParams& Filter, FDateTime LiveBefore);

	/**
	 * @brief Only include mods that have the specified tag
	 * @param Tag Tag to include
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& WithTag(UPARAM(ref) FModioFilterParams& Filter, const FString& Tag);

	/**
	 * @brief Only include mods that have all the specified tags (tag1 AND tag2 AND tagN...)
	 * @param NewTags The set of tags to filter on
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& WithTags(UPARAM(ref) FModioFilterParams& Filter, const TArray<FString>& NewTags);

	/**
	 * @brief Only include mods that do not have the specified tag
	 * @param Tag Tag to exclude
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& WithoutTag(UPARAM(ref) FModioFilterParams& Filter, const FString& Tag);

	/**
	 * @brief Only include mods that do not have any of the specified tags ( NOT (tag1 OR tag2 OR tagN...))
	 * @param NewTags Tags to exclude
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& WithoutTags(UPARAM(ref) FModioFilterParams& Filter, const TArray<FString>& NewTags);

	/**
	 * @brief Returns a sub-range of query results from StartIndex to StartIndex + ResultCount
	 * @param StartIndex Zero-based index of first result to return
	 * @param ResultCount Number of results to return
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& IndexedResults(UPARAM(ref) FModioFilterParams& Filter, int64 StartIndex,
											  int64 ResultCount);

	/**
	 * @brief Returns a sub-range of query results based on a specified page size and index
	 * @param PageNumber Zero-based index of page to return
	 * @param PageSize Number of results in a page
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& PagedResults(UPARAM(ref) FModioFilterParams& Filter, int64 PageNumber, int64 PageSize);

	/**
	 * @brief Only include mods with a metadata blob containing the specified substring
	 * @param SearchString The substring to search for
	 * @return *this
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Filter")
	static FModioFilterParams& MetadataLike(UPARAM(ref) FModioFilterParams& Filter, FString SearchString);
};
