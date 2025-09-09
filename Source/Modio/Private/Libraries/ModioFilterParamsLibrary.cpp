/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioFilterParamsLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioFilterParamsLibrary)

FModioFilterParams& UModioFilterParamsLibrary::MatchingAuthor(FModioFilterParams& Filter, const FModioUserID& ID)
{
	return Filter.MatchingAuthor(ID);
}

FModioFilterParams& UModioFilterParamsLibrary::MatchingAuthors(FModioFilterParams& Filter,
															   const TArray<FModioUserID>& IDs)
{
	return Filter.MatchingAuthors(IDs);
}

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
																   const TArray<FString>& SearchStrings)
{
	return Filter.NameContains(SearchStrings);
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
	return Filter.IndexedResults(uint64(StartIndex), uint64(ResultCount));
}

FModioFilterParams& UModioFilterParamsLibrary::PagedResults(FModioFilterParams& Filter, int64 PageNumber,
															int64 PageSize)
{
	return Filter.PagedResults(uint64(PageNumber), uint64(PageSize));
}

FModioFilterParams& UModioFilterParamsLibrary::MetadataLike(FModioFilterParams& Filter, FString SearchString)
{
	return Filter.MetadataLike(SearchString);
}

FModioFilterParams& UModioFilterParamsLibrary::DisallowMatureContent(FModioFilterParams& Filter)
{
	return Filter.DisallowMatureContent();
}

FModioFilterParams& UModioFilterParamsLibrary::WithMatureContentFlags(FModioFilterParams& Filter, int32 ByMaturity)
{
	return Filter.WithMatureContentFlags(static_cast<EModioMaturityFlags>(ByMaturity));
}

FModioFilterParams& UModioFilterParamsLibrary::RevenueType(FModioFilterParams& Filter,
														   EModioRevenueFilterType RevenueType)
{
	return Filter.RevenueType(RevenueType);
}

bool UModioFilterParamsLibrary::EqualTo(const FModioFilterParams& A, const FModioFilterParams& B)
{
	return A == B;
}

bool UModioFilterParamsLibrary::NotEqualTo(const FModioFilterParams& A, const FModioFilterParams& B)
{
	return A != B;
}
