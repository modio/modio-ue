/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Internal/Convert/FilterParams.h"
#include "Types/ModioFilterParams.h"

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
	return FString(UTF8_TO_TCHAR(ToModio(*this).ToString().c_str()));
}

FModioFilterParams& FModioFilterParams::MetadataLike(FString SearchString)
{
	MetadataBlobSearchString = SearchString;
	return *this;
}