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

FORCEINLINE Modio::FilterParams::RevenueFilterType ToModio(EModioRevenueFilterType Revenue)
{
	switch (Revenue)
	{
		case EModioRevenueFilterType::Free:
			return Modio::FilterParams::RevenueFilterType::Free;
		case EModioRevenueFilterType::Paid:
			return Modio::FilterParams::RevenueFilterType::Paid;
		case EModioRevenueFilterType::FreeAndPaid:
			return Modio::FilterParams::RevenueFilterType::FreeAndPaid;
	}

	return Modio::FilterParams::RevenueFilterType::Free;
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
		case EModioSortFieldType::DownloadsTotal:
			return Modio::FilterParams::SortFieldType::DownloadsTotal;
		case EModioSortFieldType::Alphabetical:
			return Modio::FilterParams::SortFieldType::Alphabetical;
	}

	return Modio::FilterParams::SortFieldType::ID;
}
MODIO_END_CONVERT_SWITCHES

FORCEINLINE Modio::MaturityOption ToModio(EModioMaturityFlags Maturity)
{
	return static_cast<Modio::MaturityOption>((uint8) Maturity);
}

FORCEINLINE Modio::FilterParams ToModio(const FModioFilterParams& In)
{
	Modio::FilterParams Out;

	if (In.isPaged)
	{
		Out.PagedResults(size_t(In.Index), size_t(In.Count));
	}
	else
	{
		Out.IndexedResults(size_t(In.Index), size_t(In.Count));
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

	if (In.Revenue.IsSet())
	{
		Out.RevenueType(ToModio(In.Revenue.GetValue()));
	}

	if (In.Maturity.IsSet())
	{
		Out.WithMatureContentFlags(ToModio(In.Maturity.GetValue()));
	}

	return Out.SortBy(ToModio(In.SortField), ToModio(In.Direction))
		.NameContains(ToModio(In.SearchKeywords))
		.MatchingAuthors(ToModio<Modio::UserID>(In.AuthorUserIds))
		.MatchingIDs(ToModio<Modio::ModID>(In.IncludedIDs))
		.ExcludingIDs(ToModio<Modio::ModID>(In.ExcludedIDs))
		.WithTags(ToModio(In.Tags))
		.WithoutTags(ToModio(In.ExcludedTags));
}

inline EModioSortDirection ToUnreal(const Modio::FilterParams::SortDirection& In)
{
	switch (In)
	{
		case Modio::FilterParams::SortDirection::Ascending:
			return EModioSortDirection::Ascending;
			break;
		case Modio::FilterParams::SortDirection::Descending:
			return EModioSortDirection::Descending;
			break;
		default:
			return EModioSortDirection::Descending;
			break;
	}
}
inline EModioMaturityFlags ToUnreal(const Modio::MaturityOption& In)
{
	// flag enum, so static_cast it is
	return static_cast<EModioMaturityFlags>(In);
}
inline EModioRevenueFilterType ToUnreal(const Modio::FilterParams::RevenueFilterType& In)
{
	switch (In)
	{
		case Modio::FilterParams::RevenueFilterType::Free:
			return EModioRevenueFilterType::Free;
			break;
		case Modio::FilterParams::RevenueFilterType::Paid:
			return EModioRevenueFilterType::Paid;
			break;
		case Modio::FilterParams::RevenueFilterType::FreeAndPaid:
			return EModioRevenueFilterType::FreeAndPaid;
			break;
		case Modio::FilterParams::RevenueFilterType::FreeAndPaidWeighted:
			return EModioRevenueFilterType::FreeAndPaidWeighted;
			break;
		default:
			return EModioRevenueFilterType::FreeAndPaid;
			break;
	}
}
inline EModioSortFieldType ToUnreal(const Modio::FilterParams::SortFieldType& In)
{
	switch (In)
	{
		case Modio::FilterParams::SortFieldType::ID:
			return EModioSortFieldType::ID;
			break;
		case Modio::FilterParams::SortFieldType::DownloadsToday:
			return EModioSortFieldType::DownloadsToday;
			break;
		case Modio::FilterParams::SortFieldType::SubscriberCount:
			return EModioSortFieldType::SubscriberCount;
			break;
		case Modio::FilterParams::SortFieldType::Rating:
			return EModioSortFieldType::Rating;
			break;
		case Modio::FilterParams::SortFieldType::DateMarkedLive:
			return EModioSortFieldType::DateMarkedLive;
			break;
		case Modio::FilterParams::SortFieldType::DateUpdated:
			return EModioSortFieldType::DateUpdated;
			break;
		case Modio::FilterParams::SortFieldType::DownloadsTotal:
			return EModioSortFieldType::DownloadsTotal;
			break;
		case Modio::FilterParams::SortFieldType::Alphabetical:
			return EModioSortFieldType::Alphabetical;
			break;
		default:
			return EModioSortFieldType::Alphabetical;
			break;
	}
}

inline EModioFilterTimespan ToUnreal(const Modio::FilterParams::Timespan& In)
{
	switch (In)
	{
		case Modio::FilterParams::Timespan::Today:
			return EModioFilterTimespan::Today;
			break;
		case Modio::FilterParams::Timespan::Week:
			return EModioFilterTimespan::Week;
			break;
		case Modio::FilterParams::Timespan::Month:
			return EModioFilterTimespan::Month;
			break;
		case Modio::FilterParams::Timespan::ThreeMonths:
			return EModioFilterTimespan::ThreeMonths;
			break;
		case Modio::FilterParams::Timespan::Year:
			return EModioFilterTimespan::Year;
			break;
		case Modio::FilterParams::Timespan::AllTime:
			return EModioFilterTimespan::AllTime;
			break;
		default:
			return EModioFilterTimespan::AllTime;
			break;
	}
}

struct FilterParamsDetailsAccessor : protected Modio::FilterParams
{
	FilterParamsDetailsAccessor(const Modio::FilterParams& In) : FilterParams(In) {};

	FModioFilterParams GetUnrealFilterParams()
	{
		FModioFilterParams Out;
		Out.AuthorUserIds = ToUnreal<FModioUserID>(FilterParams::IncludedAuthorUserIds);
		Out.Count = ToUnreal(FilterParams::Count);
		if (FilterParams::DateRangeBegin)
		{
			Out.DateRangeBegin = ToUnrealDateTime(*FilterParams::DateRangeBegin);
		}
		if (FilterParams::DateRangeEnd)
		{
			Out.DateRangeEnd = ToUnrealDateTime(*FilterParams::DateRangeEnd);
		}
		Out.Direction = ToUnreal(FilterParams::Direction);
		Out.ExcludedIDs = ToUnreal<FModioModID>(FilterParams::ExcludedIDs);
		Out.ExcludedTags = ToUnreal<FString>(FilterParams::ExcludedTags);
		Out.IncludedIDs = ToUnreal<FModioModID>(FilterParams::IncludedIDs);
		Out.Index = ToUnreal(FilterParams::Index);
		Out.isPaged = ToUnreal(FilterParams::IsPaged);
		if (FilterParams::Maturity)
		{
			Out.Maturity = ToUnreal(*FilterParams::Maturity);
		}
		if (FilterParams::MetadataBlobSearchString)
		{
			Out.MetadataBlobSearchString = ToUnreal(*FilterParams::MetadataBlobSearchString);
		}
		if (FilterParams::Revenue)
		{
			Out.Revenue = ToUnreal(*FilterParams::Revenue);
		}
		Out.SearchKeywords = ToUnreal<FString>(FilterParams::SearchKeywords);
		Out.SortField = ToUnreal(FilterParams::SortField);
		if (FilterParams::SpecifiedTimespan)
		{
			Out.Timespan = ToUnreal(*FilterParams::SpecifiedTimespan);
		}
		return Out;
	}
};

inline FModioFilterParams ToUnreal(const Modio::FilterParams& In)
{
	FilterParamsDetailsAccessor Access {In};
	return Access.GetUnrealFilterParams();
}
