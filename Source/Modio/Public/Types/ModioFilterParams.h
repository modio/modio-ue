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

/**
 * @docpublic
 * @brief Enum indicating which field should be used to sort the results
 */
UENUM(BlueprintType)
enum class EModioSortFieldType : uint8
{
	/** Use mod ID (default) */
	ID,
	/** Use number of downloads in last 24 (exposed in REST API as "popular") */
	DownloadsToday,
	/** Use number of subscribers */
	SubscriberCount,
	/** Use mod rating */
	Rating,
	/** Use date mod was marked live */
	DateMarkedLive,
	/** Use date mod was last updated */
	DateUpdated,
	/** Use downloads total */
	DownloadsTotal,
	/** Use mod name */
	Alphabetical
};

/**
 * @docpublic
 * @brief Enum indicating which direction sorting should be applied
 */
UENUM(BlueprintType)
enum class EModioSortDirection : uint8
{
	/** (default) */
	Ascending,
	/** Sort in descending order */
	Descending
};

/**
 * @docpublic
 * @brief Enum indicating filtering options based off revenue type
 */
UENUM(BlueprintType)
enum class EModioRevenueFilterType : uint8
{
	/** Return only free mods */
	Free = 0,
	/** Return only paid mods */
	Paid = 1,
	/** Return both free and paid mods */
	FreeAndPaid = 2
};

/**
 * @docpublic
 * @brief Class storing a set of filter parameters for use in [`ListAllModsAsync`](#listallmodsasync)
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioFilterParams
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Only include mods with the specified author User ID
	 * @param UserID The User ID to filter on
	 * @return *this
	 */
	FModioFilterParams& MatchingAuthor(const FModioUserID& UserID);

	/**
	 * @docpublic
	 * @brief Only include mods with the specified author User IDs
	 * @param UserIDs The list of User IDs to filter on
	 * @return *this
	 */
	FModioFilterParams& MatchingAuthors(const TArray<FModioUserID>& UserIDs);

	/**
	 * @docpublic
	 * @brief Indicates the filter should only include the specified mods
	 * @param IDs the set of mods to match
	 * @return *this
	 */
	FModioFilterParams& MatchingIDs(const TArray<FModioModID>& IDs);

	/**
	 * @docpublic
	 * @brief Indicates the filter should exclude the specified mods
	 * @param IDs the set of mods to exclude
	 * @return *this
	 */
	FModioFilterParams& ExcludingIDs(const TArray<FModioModID>& IDs);

	/**
	 * @docpublic
	 * @brief Indicates results should be sorted using the specified field and direction
	 * @param ByField Field to sort with
	 * @param ByDirection Direction to sort
	 * @return *this
	 */
	FModioFilterParams& SortBy(EModioSortFieldType ByField, EModioSortDirection ByDirection);

	/**
	 * @docpublic
	 * @brief Only include mods where the name contains the provided string
	 * @param SearchString Search string
	 * @return *this
	 */
	FModioFilterParams& NameContains(const FString& SearchString);

	/**
	 * @docpublic
	 * @brief Only include mods where the name contains at least one of the provided strings (string1 OR string2 OR
	 * stringN...)
	 * @param SearchString First search string
	 * @return *this
	 */
	FModioFilterParams& NameContains(const TArray<FString>& SearchString);

	/**
	 * @docpublic
	 * @brief Only include mods that were marked live (i.e released) after the specified date
	 * @param LiveAfter Minimum date
	 * @return *this
	 */
	FModioFilterParams& MarkedLiveAfter(FDateTime LiveAfter);

	/**
	 * @docpublic
	 * @brief Only include mods that were marked live (i.e released) before the specified date
	 * @param LiveBefore Maximum date
	 * @return *this
	 */
	FModioFilterParams& MarkedLiveBefore(FDateTime LiveBefore);

	/**
	 * @docpublic
	 * @brief Only include mods with a metadata blob containing the specified substring
	 * @param SearchString The substring to search for
	 * @return *this
	 */
	FModioFilterParams& MetadataLike(FString SearchString);

	/**
	 * @docpublic
	 * @brief Only include mods that have the specified tag
	 * @param Tag Tag to include
	 * @return *this
	 */
	FModioFilterParams& WithTags(const FString& Tag);

	/**
	 * @docpublic
	 * @brief Only include mods that have all the specified tags (tag1 AND tag2 AND tagN...)
	 * @param NewTags The set of tags to filter on
	 * @return *this
	 */
	FModioFilterParams& WithTags(const TArray<FString>& NewTags);

	/**
	 * @docpublic
	 * @brief Only include mods that do not have the specified tag
	 * @param Tag Tag to exclude
	 * @return *this
	 */
	FModioFilterParams& WithoutTags(const FString& Tag);

	/**
	 * @docpublic
	 * @brief Only include mods that do not have any of the specified tags ( NOT (tag1 OR tag2 OR tagN...))
	 * @param NewTags Tags to exclude
	 * @return *this
	 */
	FModioFilterParams& WithoutTags(const TArray<FString>& NewTags);

	/**
	 * @docpublic
	 * @brief Returns a sub-range of query results from StartIndex to StartIndex + ResultCount
	 * @param StartIndex Zero-based index of first result to return
	 * @param ResultCount Number of results to return
	 * @return *this
	 */
	FModioFilterParams& IndexedResults(uint64 StartIndex, uint64 ResultCount);

	/**
	 * @docpublic
	 * @brief Returns a sub-range of query results based on a specified page size and index
	 * @param PageNumber Zero-based index of page to return
	 * @param PageSize Number of results in a page
	 * @return *this
	 */
	FModioFilterParams& PagedResults(uint64 PageNumber, uint64 PageSize);

	/**
	 * @docpublic
	 * @brief Returned mods according to the specified revenue type (free, paid, both)
	 * @param RevenueFilter Filter to use
	 * @return *this
	 */
	FModioFilterParams& RevenueType(EModioRevenueFilterType RevenueFilter);

	/**
	 * @docpublic
	 * @brief Indicates results should exclude all mods which contain mature content
	 * @return *this
	 */
	FModioFilterParams& DisallowMatureContent();

	/**
	 * @docpublic
	 * @brief Indicates results should be filtered by maturity options
	 * @param ByMaturity Maturity flags to filter by
	 * @return *this
	 */
	FModioFilterParams& WithMatureContentFlags(EModioMaturityFlags ByMaturity);

	/**
	 * @docpublic
	 * @brief Converts the filter params to a string suitable for use in the REST API.
	 * @note Performs allocation to acquire the string
	 * @return FString containing the filter parameters
	 */
	FString ToString() const;


	/**
	 * @docpublic
	 * @brief Comparison operator between ModioFilterParams elements
	 */
	MODIO_API friend bool operator==(const FModioFilterParams& A, const FModioFilterParams& B)
	{
		return A.Direction == B.Direction && A.SortField == B.SortField && A.SearchKeywords == B.SearchKeywords &&
			   A.DateRangeBegin == B.DateRangeBegin && A.DateRangeEnd == B.DateRangeEnd && A.Tags == B.Tags &&
			   A.ExcludedTags == B.ExcludedTags && A.AuthorUserIds == B.AuthorUserIds && A.IncludedIDs == B.IncludedIDs &&
			   A.ExcludedIDs == B.ExcludedIDs && A.MetadataBlobSearchString == B.MetadataBlobSearchString &&
			   A.Revenue == B.Revenue && A.Maturity == B.Maturity && A.isPaged == B.isPaged && A.Index == B.Index &&
			   A.Count == B.Count;
	}

	/**
	 * @docpublic
	 * @brief Comparison operator between ModioFilterParams elements
	 */
	MODIO_API friend bool operator!=(const FModioFilterParams& A, const FModioFilterParams& B)
	{
		return !(A == B);
	}

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
 * @docpublic
 * @brief Helper struct for named preset filter parameters
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioPresetFilterParams
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Preset name
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	FText PresetName;

	/**
	 * @docpublic
	 * @brief List of tags to filter on
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	TArray<FString> Tags;

	/**
	 * @docpublic
	 * @brief List of tags to exclude from filtering
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	TArray<FString> ExcludedTags;

	/**
	 * @docpublic
	 * @brief Sorting direction (default is ascending)
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	EModioSortDirection Direction = EModioSortDirection::Ascending;

	/**
	 * @docpublic
	 * @brief Field to use for sorting (default is ID)
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	EModioSortFieldType SortField = EModioSortFieldType::ID;

	/**
	 * @docpublic
	 * @brief Number of results to return
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FModioPresetFilterParams")
	int64 Count = 20;

	/**
	 * @docpublic
	 * @brief Converts the preset filter into actual filter parameters
	 * @return The converted filter parameters
	 */
	FModioFilterParams ToFilterParams() const
	{
		return FModioFilterParams()
			.WithTags(Tags)
			.WithoutTags(ExcludedTags)
			.SortBy(SortField, Direction)
			.IndexedResults(0, uint64_t(Count));
	}
};

/**
 * @docpublic
 * @brief Blueprint library for working with preset filter parameters
 */
UCLASS()
class MODIO_API UModioPresetFilterParamsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

protected:
	/**
	 * @docpublic
	 * @brief Converts a preset filter into a concrete set of filter parameters that can be passed to the mod.io plugin
	 * @param Preset The preset to convert
	 * @return The converted filter params
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Filter Params")
	static FModioFilterParams ToFilterParams(const FModioPresetFilterParams& Preset)
	{
		return Preset.ToFilterParams();
	}
};
