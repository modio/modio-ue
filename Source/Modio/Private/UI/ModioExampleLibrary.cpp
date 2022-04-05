/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "UI/ModioExampleLibrary.h"
#include "ModioSubsystem.h"
#include "Async/Async.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include <system_error>
#include "ModioSDK.h"

EModioAvatarSize UModioExampleLibrary::GetAvatarThumbnailSize()
{
	return EModioAvatarSize::Original;
}

EModioLogoSize UModioExampleLibrary::GetLogoThumbnailSize()
{
	return EModioLogoSize::Original;
}

EModioLogoSize UModioExampleLibrary::GetLogoFullSize()
{
	return EModioLogoSize::Original;
}

static void ParseIntoMap(const TArray<FString>& KeyValuePairs, TMap<FString, FString>& OptionsMap)
{
	for (auto& KVP : KeyValuePairs)
	{
		FString Key, Value;
		if (KVP.Split(TEXT("="), &Key, &Value, ESearchCase::CaseSensitive))
		{
			OptionsMap.Add(Key, Value);
		}
	}
}

static TOptional<FString> GetStringOption(const TMap<FString, FString>& OptionsMap, const FString& Key)
{
	if (const FString* Value = OptionsMap.Find(Key))
	{
		return *Value;
	}
	return {};
}

static TOptional<int32> GetIntOption(const TMap<FString, FString>& OptionsMap, const FString& Key)
{
	if (const FString* Value = OptionsMap.Find(Key))
	{
		return FCString::Atoi(**Value);
	}
	return {};
}

static TOptional<TFunction<bool(const FModioModInfo& A, const FModioModInfo& B)>> GetSortOption(
	const TMap<FString, FString>& OptionsMap)
{
	TOptional<FString> SortBy = GetStringOption(OptionsMap, TEXT("_sort"));
	if (!SortBy)
	{
		return {};
	}

	bool bSortAcending = true;
	if (SortBy->RemoveFromStart(TEXT("-"), ESearchCase::CaseSensitive))
	{
		bSortAcending = false;
	}

// Warning: pasting formed an invalid preprocessing token
// https://stackoverflow.com/questions/46356185/pasting-formed-an-invalid-processing-token
#define SORT_FUNC(SORT_ASC, VARIABLE)                                         \
	(SORT_ASC) ? TFunction<bool(const FModioModInfo&, const FModioModInfo&)>( \
					 [](const FModioModInfo& First, const FModioModInfo& Second) {     \
						 if (First.VARIABLE < Second.VARIABLE)                     \
						 {                                                    \
							 return true;                                     \
						 }                                                    \
						 return First.ModId < Second.ModId;                            \
					 })                                                       \
			   : TFunction<bool(const FModioModInfo&, const FModioModInfo&)>( \
					 [](const FModioModInfo& First, const FModioModInfo& Second) {     \
						 if (First.VARIABLE > Second.VARIABLE)                     \
						 {                                                    \
							 return true;                                     \
						 }                                                    \
						 return First.ModId > Second.ModId;                            \
					 })

	// @todo: Optimize: Do the select first
	TOptional<TFunction<bool(const FModioModInfo&, const FModioModInfo&)>> SortFunc = {};
	if (SortBy->Equals(TEXT("date_live")))
	{
		SortFunc = SORT_FUNC(bSortAcending, ProfileDateLive);
	}
	else if (SortBy->Equals(TEXT("date_updated")))
	{
		SortFunc = SORT_FUNC(bSortAcending, ProfileDateUpdated);
	}
	else if (SortBy->Equals(TEXT("popular")))
	{
		// We don't have this data, so we can't sort on popular, so do no sorting
	}
	else if (SortBy->Equals(TEXT("rating")))
	{
		// Has inverted sorting on backend
		bSortAcending = !bSortAcending;
		SortFunc = SORT_FUNC(bSortAcending, Stats.RatingWeightedAggregate);
	}
	else if (SortBy->Equals(TEXT("subscribers")))
	{
		// Has inverted sorting on backend
		bSortAcending = !bSortAcending;
		SortFunc = SORT_FUNC(bSortAcending, Stats.SubscribersTotal);
	}
	else if (SortBy->Equals(TEXT("id")))
	{
		SortFunc = bSortAcending
					   ? TFunction<bool(const FModioModInfo&, const FModioModInfo&)>(
							 [](const FModioModInfo& First, const FModioModInfo& Second) { return First.ModId < Second.ModId; })
					   : TFunction<bool(const FModioModInfo&, const FModioModInfo&)>(
							 [](const FModioModInfo& First, const FModioModInfo& Second) { return First.ModId > Second.ModId; });
	}
	else
	{
		//UE_LOG(LogModioExample, Warning, TEXT("Unknown sort field %s"), *SortBy.GetValue());
	}

#undef SORT_FUNC

	return SortFunc;
}

void UModioExampleLibrary::ListUserSubscriptionAsync(const FModioFilterParams& FilterParams,
													 FOnListAllModsDelegate Callback)
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!IsValid(Subsystem))
	{
		// @todo: Exchange this for a mod.io internal error when we have one in the SDK
		Callback.ExecuteIfBound(FModioErrorCode(Modio::ErrorCode(std::make_error_code(std::errc::not_supported))), {});
		return;
	}

	// When API is threadsafe, then we can do this query on the background thread and avoid more hitches
	auto UserSubscriptionsGT = Subsystem->QueryUserSubscriptions();

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [UserSubscriptions = MoveTemp(UserSubscriptionsGT),
															 FilterParams, Callback]() {
		TArray<FModioModInfo> ModList;
		ModList.Reserve(UserSubscriptions.Num());

		FString FilterString = FilterParams.ToString();
		TArray<FString> KeyValuePairs;
		FilterString.ParseIntoArray(KeyValuePairs, TEXT("&"), true);
		TMap<FString, FString> OptionMap;
		ParseIntoMap(KeyValuePairs, OptionMap);

		const TOptional<int32> PageSize = GetIntOption(OptionMap, TEXT("_limit"));
		const TOptional<int32> PageOffset = GetIntOption(OptionMap, TEXT("_offset"));
		TOptional<FString> SearchTerm = GetStringOption(OptionMap, TEXT("_q"));
		TOptional<FString> TagFilter = GetStringOption(OptionMap, TEXT("tags"));

		const int32 StartIndex = PageOffset.Get(0);
		const int32 EndIndex = StartIndex + PageSize.Get(100);

		// Generate array for sorting
		for (const auto& It : UserSubscriptions)
		{
			// Filtering
			{
				// Filter on SearchTerm
				const FModioModInfo& ModInfo = It.Value.GetModProfile();
				if (SearchTerm.IsSet())
				{
					// Do the most simplistic searching with just contains
					if (!ModInfo.ProfileName.Contains(SearchTerm.GetValue()))
					{
						continue;
					}
				}
				// Filter on tags
				if (TagFilter.IsSet())
				{
					// Do the most simplistic searching with just contains
					if (!ModInfo.Tags.ContainsByPredicate(
							[Tag = TagFilter.GetValue()](const FModioModTag& ItemTag) { return ItemTag.Tag.Equals(Tag); }))
					{
						continue;
					}
				}
			}
			ModList.Add(It.Value.GetModProfile());
		}

		// Select sort function depending on option
		auto Sorting = GetSortOption(OptionMap);
		if (Sorting)
		{
			ModList.Sort(Sorting.GetValue());
		}

		const int32 ResultsFound = ModList.Num();

		if (EndIndex < ModList.Num())
		{
			ModList.SetNum(EndIndex, false);
		}
		if (StartIndex > 0)
		{
			ModList.RemoveAt(0, StartIndex);
		}

		const FModioPagedResult PagedResult(StartIndex, PageSize.Get(100), ResultsFound, ResultsFound);

		AsyncTask(ENamedThreads::GameThread, [Callback, ModCollection = MoveTemp(ModList), PagedResult]() mutable {
			const FModioModInfoList ModInfoList(PagedResult, MoveTemp(ModCollection));
			Callback.ExecuteIfBound({}, FModioOptionalModInfoList(TOptional<FModioModInfoList>(ModInfoList)));
		});
	});
}
