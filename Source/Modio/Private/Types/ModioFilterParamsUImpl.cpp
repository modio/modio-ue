/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Internal/Convert/FilterParams.h"
#include "Types/ModioFilterParams.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioFilterParams)

FModioFilterParams& FModioFilterParams::MatchingAuthor(const FModioUserID& UserID)
{
	AuthorUserIds.Empty();
	AuthorUserIds.Add(UserID);
	return *this;
}

FModioFilterParams& FModioFilterParams::MatchingAuthors(const TArray<FModioUserID>& UserIDs)
{
	AuthorUserIds = UserIDs;
	return *this;
}

FModioFilterParams& FModioFilterParams::MatchingIDs(const TArray<FModioModID>& IDs)
{
	IncludedIDs = IDs;
	return *this;
}

FModioFilterParams& FModioFilterParams::ExcludingIDs(const TArray<FModioModID>& IDs)
{
	ExcludedIDs = IDs;
	return *this;
}

FModioFilterParams& FModioFilterParams::SortBy(EModioSortFieldType ByField, EModioSortDirection ByDirection)
{
	SortField = ByField;
	Direction = ByDirection;
	return *this;
}

FModioFilterParams& FModioFilterParams::NameContains(const FString& SearchString)
{
	SearchKeywords.Empty();
	SearchKeywords.Add(SearchString);
	return *this;
}

FModioFilterParams& FModioFilterParams::NameContains(const TArray<FString>& SearchString)
{
	SearchKeywords = SearchString;
	return *this;
}

FModioFilterParams& FModioFilterParams::MarkedLiveAfter(FDateTime LiveAfter)
{
	DateRangeBegin = LiveAfter;
	return *this;
}

FModioFilterParams& FModioFilterParams::MarkedLiveBefore(FDateTime LiveBefore)
{
	DateRangeEnd = LiveBefore;
	return *this;
}

FModioFilterParams& FModioFilterParams::WithTags(const FString& Tag)
{
	Tags.Empty();
	Tags.Add(Tag);
	return *this;
}

FModioFilterParams& FModioFilterParams::WithTags(const TArray<FString>& NewTags)
{
	Tags = NewTags;
	return *this;
}

FModioFilterParams& FModioFilterParams::WithoutTags(const FString& Tag)
{
	ExcludedTags.Empty();
	ExcludedTags.Add(Tag);
	return *this;
}

FModioFilterParams& FModioFilterParams::WithoutTags(const TArray<FString>& NewTags)
{
	ExcludedTags = NewTags;
	return *this;
}

FModioFilterParams& FModioFilterParams::IndexedResults(uint64 StartIndex, uint64 ResultCount)
{
	isPaged = false;
	Index = StartIndex;
	Count = ResultCount;
	return *this;
}

FModioFilterParams& FModioFilterParams::PagedResults(uint64 PageNumber, uint64 PageSize)
{
	isPaged = true;
	Index = PageNumber;
	Count = PageSize;
	return *this;
}

FString FModioFilterParams::ToString() const
{
	FString FilterParamString;

	for (auto& FilterParam : ToModio(*this).ToQueryParamaters())
	{
		FilterParamString += FString::Printf(TEXT("%s=%s&"), UTF8_TO_TCHAR(FilterParam.first.c_str()), UTF8_TO_TCHAR(FilterParam.second.c_str()));

	}
	return FilterParamString;
}

FModioFilterParams& FModioFilterParams::MetadataLike(FString SearchString)
{
	MetadataBlobSearchString = SearchString;
	return *this;
}

FModioFilterParams& FModioFilterParams::DisallowMatureContent()
{
	Maturity = EModioMaturityFlags::None;
	return *this;
}

FModioFilterParams& FModioFilterParams::WithMatureContentFlags(EModioMaturityFlags ByMaturity)
{
	Maturity = ByMaturity;
	return *this;
}

FModioFilterParams& FModioFilterParams::RevenueType(EModioRevenueFilterType RevenueFilter)
{
	Revenue = RevenueFilter;
	return *this;
}
