#pragma once

#include "Internal/ModioConvert.h"
#include "Internal/ModioPrivateDefines.h"

#include "ModioSDK.h"

#include "Types/ModioCommonTypes.h"

// clang-format off
#include "ModioFilterParams.generated.h"
// clang-format on

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

FORCEINLINE Modio::FilterParams::SortFieldType ToModio(EModioSortFieldType Field);
FORCEINLINE Modio::FilterParams::SortDirection ToModio(EModioSortDirection Direction);

/** @brief Class storing a set of filter parameters for use in xref:ListAllModsAsync[] */
USTRUCT(BlueprintType)
struct MODIO_API FModioFilterParams
{
	GENERATED_BODY()

	/** Convert to the underlying type for the modio functions */
	FORCEINLINE const Modio::FilterParams& operator*() const
	{
		return FilterParams;
	}

	/**
	 * @brief Indicates the filter should only include the specified mods
	 * @param IDs the set of mods to match
	 * @return *this
	 */
	FModioFilterParams& MatchingIDs(const TArray<FModioModID>& IDs)
	{
		std::vector<Modio::ModID> Out;
		for (FModioModID ID : IDs)
		{
			Out.push_back(ID);
		}

		FilterParams.MatchingIDs(Out);
		return *this;
	}

	/**
	 * @brief Indicates the filter should exclude the specified mods.
	 * @param IDs the set of mods to exclude
	 * @return *this
	 */
	FModioFilterParams& ExcludingIDs(const TArray<FModioModID>& IDs)
	{
		std::vector<Modio::ModID> Out;
		for (FModioModID ID : IDs)
		{
			Out.push_back(ID);
		}

		FilterParams.ExcludingIDs(Out);
		return *this;
	}

	/**
	 * @brief Indicates results should be sorted using the specified field and direction
	 * @param ByField Field to sort with
	 * @param ByDirection Direction to sort
	 * @return *this
	 **/
	FORCEINLINE FModioFilterParams& SortBy(EModioSortFieldType ByField, EModioSortDirection ByDirection)
	{
		FilterParams.SortBy(ToModio(ByField), ToModio(ByDirection));
		return *this;
	}

	/**
	 * @brief Only include mods where the name contains the provided string
	 * @param SearchString Search string
	 * @return *this
	 **/
	FORCEINLINE FModioFilterParams& NameContains(const FString& SearchString)
	{
		FilterParams.NameContains(ToSTD(SearchString));
		return *this;
	}

	/**
	 * @brief Only include mods where the name contains at least one of the provided strings (string1 OR string2 OR
	 *stringN...)
	 * @tparam ...Args std::string
	 * @param SearchString First search string
	 * @param ...args Additional search strings
	 * @return *this
	 **/
	FORCEINLINE FModioFilterParams& NameContains(const TArray<FString>& SearchString)
	{
		FilterParams.NameContains(ToSTD(SearchString));
		return *this;
	}

	/**
	 * @brief Only include mods that were marked live (i.e released) after the specified date
	 * @param LiveAfter Minimum date
	 * @return *this
	 **/
	FORCEINLINE FModioFilterParams& MarkedLiveAfter(FDateTime LiveAfter)
	{
		FilterParams.MarkedLiveAfter(ToSTD(LiveAfter));
		return *this;
	}

	/**
	 * @brief Only include mods that were marked live (i.e released) before the specified date
	 * @param LiveBefore Maximum date
	 * @return *this
	 **/
	FORCEINLINE FModioFilterParams& MarkedLiveBefore(FDateTime LiveBefore)
	{
		FilterParams.MarkedLiveBefore(ToSTD(LiveBefore));
		return *this;
	}

	/**
	 * @brief Only include mods that have the specified tag
	 * @param Tag Tag to include
	 * @return *this
	 **/
	FORCEINLINE FModioFilterParams& WithTags(const FString& Tag)
	{
		FilterParams.WithTags(ToSTD(Tag));
		return *this;
	}

	/**
	 * @brief Only include mods that have all the specified tags (tag1 AND tag2 AND tagN...)
	 * @param NewTags The set of tags to filter on
	 * @return *this
	 **/
	FORCEINLINE FModioFilterParams& WithTags(const TArray<FString>& NewTags)
	{
		FilterParams.WithTags(ToSTD(NewTags));
		return *this;
	}

	/**
	 * @brief Only include mods that do not have the specified tag
	 * @param Tag Tag to exclude
	 * @return *this
	 **/
	FORCEINLINE FModioFilterParams& WithoutTags(const FString& Tag)
	{
		FilterParams.WithoutTags(ToSTD(Tag));
		return *this;
	}

	/**
	 * @brief Only include mods that do not have any of the specified tags ( NOT (tag1 OR tag2 OR tagN...))
	 * @param NewTags Tags to exclude
	 * @return *this
	 **/
	FORCEINLINE FModioFilterParams& WithoutTags(const TArray<FString>& NewTags)
	{
		FilterParams.WithoutTags(ToSTD(NewTags));
		return *this;
	}

	/**
	 * @brief Returns a sub-range of query results from StartIndex to StartIndex + ResultCount
	 * @param StartIndex Zero-based index of first result to return
	 * @param ResultCount Number of results to return
	 * @return *this
	 **/
	FORCEINLINE FModioFilterParams& IndexedResults(uint64 StartIndex, uint64 ResultCount)
	{
		FilterParams.IndexedResults(StartIndex, ResultCount);
		return *this;
	}

	/**
	 * @brief Returns a sub-range of query results based on a specified page size and index
	 * @param PageNumber Zero-based index of page to return
	 * @param PageSize Number of results in a page
	 * @return
	 **/
	FORCEINLINE FModioFilterParams& PagedResults(uint64 PageNumber, uint64 PageSize)
	{
		FilterParams.PagedResults(PageNumber, PageSize);
		return *this;
	}

	/**
	 *  @brief Converts the filter params to a string suitable for use in the REST API.
	 *  @note Performs a allocation to acquire the string
	 *  @return FString containing the filter parameters
	 */
	FORCEINLINE FString ToString() const
	{
		return FString(UTF8_TO_TCHAR(FilterParams.ToString().c_str()));
	}

private:
	Modio::FilterParams FilterParams;
};

#pragma region ToModio implementation
MODIO_BEGIN_CONVERT_SWITCHES
Modio::FilterParams::SortDirection ToModio(EModioSortDirection SortDirection)
{
	switch (SortDirection)
	{
		case EModioSortDirection::Ascending:
			return Modio::FilterParams::SortDirection::Ascending;
		case EModioSortDirection::Descending:
			return Modio::FilterParams::SortDirection::Descending;
	}

	return Modio::FilterParams::SortDirection::Ascending;
}

Modio::FilterParams::SortFieldType ToModio(EModioSortFieldType Environment)
{
	switch (Environment)
	{
		case EModioSortFieldType::ID:
			return Modio::FilterParams::SortFieldType::ID;
		case EModioSortFieldType::DownloadsToday:
			return Modio::FilterParams::SortFieldType::DownloadsToday;
		case EModioSortFieldType::SubscriberCount:
			return Modio::FilterParams::SortFieldType::SubscriberCount;
		case EModioSortFieldType::Rating:
			return Modio::FilterParams::SortFieldType::Rating;
		case EModioSortFieldType::DateMarkedLive:
			return Modio::FilterParams::SortFieldType::DateMarkedLive;
		case EModioSortFieldType::DateUpdated:
			return Modio::FilterParams::SortFieldType::DateUpdated;
	}

	return Modio::FilterParams::SortFieldType::ID;
}
MODIO_END_CONVERT_SWITCHES
#pragma endregion