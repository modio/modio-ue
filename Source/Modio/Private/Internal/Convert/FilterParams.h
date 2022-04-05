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
#include "Internal/ModioConvert.h"
#include "Internal/ModioPrivateDefines.h"
#include "ModioSDK.h"
#include "Types/ModioFilterParams.h"

MODIO_BEGIN_CONVERT_SWITCHES
FORCEINLINE Modio::FilterParams::SortDirection ToModio(EModioSortDirection SortDirection)
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

FORCEINLINE Modio::FilterParams::SortFieldType ToModio(EModioSortFieldType Environment)
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

FORCEINLINE Modio::FilterParams ToModio(const FModioFilterParams& In)
{
	Modio::FilterParams Out;

	if (In.isPaged)
	{
		Out.PagedResults(In.Index, In.Count);
	}
	else
	{
		Out.IndexedResults(In.Index, In.Count);
	}

	if (In.DateRangeBegin)
	{
		Out.MarkedLiveAfter(ToModioDateTime(In.DateRangeBegin.GetValue()));
	}

	if (In.DateRangeEnd)
	{
		Out.MarkedLiveBefore(ToModioDateTime(In.DateRangeEnd.GetValue()));
	}

	if (In.MetadataBlobSearchString.IsSet())
	{
		Out.MetadataLike(ToModio(In.MetadataBlobSearchString.GetValue()));
	}

	return Out.SortBy(ToModio(In.SortField), ToModio(In.Direction))
		.NameContains(ToModio(In.SearchKeywords))
		.MatchingIDs(ToModio<Modio::ModID>(In.IncludedIDs))
		.ExcludingIDs(ToModio<Modio::ModID>(In.ExcludedIDs))
		.WithTags(ToModio(In.Tags))
		.WithoutTags(ToModio(In.ExcludedTags));
}