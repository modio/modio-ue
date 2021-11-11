/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioFilterParamsLibrary.h"

FModioFilterParams& UModioFilterParamsLibrary::SortBy(FModioFilterParams& Filter, EModioSortFieldType ByField,
													  EModioSortDirection ByDirection)
{
	return Filter.SortBy(ByField, ByDirection);
}

FModioFilterParams& UModioFilterParamsLibrary::NameContains(FModioFilterParams& Filter, const FString& SearchString)
{
	return Filter.NameContains(SearchString);
}

FModioFilterParams& UModioFilterParamsLibrary::NameContainsStrings(FModioFilterParams& Filter,
																   const FString& SearchString)
{
	return Filter.NameContains(SearchString);
}

FModioFilterParams& UModioFilterParamsLibrary::MarkedLiveAfter(FModioFilterParams& Filter, FDateTime LiveAfter)
{
	return Filter.MarkedLiveAfter(LiveAfter);
}

FModioFilterParams& UModioFilterParamsLibrary::MarkedLiveBefore(FModioFilterParams& Filter, FDateTime LiveBefore)
{
	return Filter.MarkedLiveBefore(LiveBefore);
}

FModioFilterParams& UModioFilterParamsLibrary::WithTag(FModioFilterParams& Filter, const FString& Tag)
{
	return Filter.WithTags(Tag);
}

FModioFilterParams& UModioFilterParamsLibrary::WithTags(FModioFilterParams& Filter, const TArray<FString>& NewTags)
{
	return Filter.WithTags(NewTags);
}

FModioFilterParams& UModioFilterParamsLibrary::WithoutTags(FModioFilterParams& Filter, const TArray<FString>& NewTags)
{
	return Filter.WithoutTags(NewTags);
}

FModioFilterParams& UModioFilterParamsLibrary::WithoutTag(FModioFilterParams& Filter, const FString& Tag)
{
	return Filter.WithoutTags(Tag);
}
FModioFilterParams& UModioFilterParamsLibrary::IndexedResults(FModioFilterParams& Filter, int64 StartIndex,
															  int64 ResultCount)
{
	return Filter.IndexedResults(StartIndex, ResultCount);
}

FModioFilterParams& UModioFilterParamsLibrary::PagedResults(FModioFilterParams& Filter, int64 PageNumber,
															int64 PageSize)
{
	return Filter.PagedResults(PageNumber, PageSize);
}

FModioFilterParams& UModioFilterParamsLibrary::MetadataLike(FModioFilterParams& Filter, FString SearchString)
{
	return Filter.MetadataLike(SearchString);
}
