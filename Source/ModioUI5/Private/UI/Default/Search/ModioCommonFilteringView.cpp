/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/Search/ModioCommonFilteringView.h"

#include "ModioSubsystem.h"
#include "ModioUI5.h"
#include "Components/PanelWidget.h"
#include "Engine/Engine.h"
#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"
#include "UI/Foundation/Base/Search/ModioCommonSearchViewBase.h"
#include "UI/Foundation/Components/Tag/ModioCommonModTagGroupList.h"
#include "UI/Foundation/Components/Tag/ModioCommonModTagList.h"

const TMap<TTuple<EModioSortFieldType, EModioSortDirection>, FText> SortFieldAndSortDirectionMap = []() {
	TMap<TTuple<EModioSortFieldType, EModioSortDirection>, FText> InternalSortFieldAndSortDirectionMap;
	InternalSortFieldAndSortDirectionMap.Add({EModioSortFieldType::DownloadsToday, EModioSortDirection::Descending}, NSLOCTEXT("Modio", "TrendingOptionText", "Trending"));
	InternalSortFieldAndSortDirectionMap.Add({EModioSortFieldType::Rating, EModioSortDirection::Descending}, NSLOCTEXT("Modio", "HighestRatedOptionText", "Highest Rated"));
	InternalSortFieldAndSortDirectionMap.Add({EModioSortFieldType::DownloadsTotal, EModioSortDirection::Descending}, NSLOCTEXT("Modio", "MostPopularOptionText", "Most Popular"));
	InternalSortFieldAndSortDirectionMap.Add({EModioSortFieldType::SubscriberCount, EModioSortDirection::Descending}, NSLOCTEXT("Modio", "MostSubscribersOptionText", "Most Subscribers"));
	InternalSortFieldAndSortDirectionMap.Add({EModioSortFieldType::DateMarkedLive, EModioSortDirection::Descending}, NSLOCTEXT("Modio", "RecentlyAddedOptionText", "Recently Added"));
	InternalSortFieldAndSortDirectionMap.Add({EModioSortFieldType::DateUpdated, EModioSortDirection::Descending}, NSLOCTEXT("Modio", "LastUpdatedOptionText", "Last Updated"));
	return InternalSortFieldAndSortDirectionMap;
}();

const TMap<EModioManualSortType, FText> ManualSortFieldMap = []() {
	TMap<EModioManualSortType, FText> InternalManualSortFieldMap;
	InternalManualSortFieldMap.Add(EModioManualSortType::AToZ, NSLOCTEXT("Modio", "AToZ", "Alphabetical (A-Z)"));
	InternalManualSortFieldMap.Add(EModioManualSortType::ZToA, NSLOCTEXT("Modio", "ZToA", "Alphabetical (Z-A)"));
	InternalManualSortFieldMap.Add(EModioManualSortType::SizeOnDisk, NSLOCTEXT("Modio", "SizeOnDisk", "File size"));
	InternalManualSortFieldMap.Add(EModioManualSortType::RecentlyUpdated, NSLOCTEXT("Modio", "RecentlyUpdated", "Recently updated"));
	return InternalManualSortFieldMap;
}();

const TMap<EModioEnabledFilterType, FText> EnabledFilterFieldMap = []() {
	TMap<EModioEnabledFilterType, FText> InternalEnabledFilterFieldMap;
	InternalEnabledFilterFieldMap.Add(EModioEnabledFilterType::Enabled, NSLOCTEXT("Modio", "Enabled", "Enabled"));
	InternalEnabledFilterFieldMap.Add(EModioEnabledFilterType::Disabled, NSLOCTEXT("Modio", "Disabled", "Disabled"));
	return InternalEnabledFilterFieldMap;
}();

const TMap<EModioInstalledFilterType, FText> InstalledFilterFieldMap = []() {
	TMap<EModioInstalledFilterType, FText> InternalInstalledFilterFieldMap;
	InternalInstalledFilterFieldMap.Add(EModioInstalledFilterType::CurrentUser, NSLOCTEXT("Modio", "InstalledByCurrentUserTagValue", "Current User"));
	InternalInstalledFilterFieldMap.Add(EModioInstalledFilterType::AnotherUser, NSLOCTEXT("Modio", "InstalledByAnotherUserTagValue", "Another User"));
	return InternalInstalledFilterFieldMap;
}();

const FModioModTagInfo SortFieldAndSortDirectionTagInfo = []() {
	FModioModTagInfo InternalSortFieldAndSortDirectionTagInfo;
	InternalSortFieldAndSortDirectionTagInfo.bAllowMultipleSelection = false;
	InternalSortFieldAndSortDirectionTagInfo.TagGroupName = NSLOCTEXT("Modio", "SortFieldAndSortDirectionTagGroupName", "Sort by").ToString();
	for (auto& SortFieldAndDirection : SortFieldAndSortDirectionMap)
	{
		InternalSortFieldAndSortDirectionTagInfo.TagGroupValues.Add(SortFieldAndDirection.Value.ToString());
	}
	return InternalSortFieldAndSortDirectionTagInfo;
}();

const FModioModTagInfo ManualSortFieldTagInfo = []() {
	FModioModTagInfo InternalManualSortFieldTagInfo;
	InternalManualSortFieldTagInfo.bAllowMultipleSelection = false;
	InternalManualSortFieldTagInfo.TagGroupName = NSLOCTEXT("Modio", "ManualSortFieldTagGroupName", "Sort by").ToString();
	for (auto& SortField : ManualSortFieldMap)
	{
		InternalManualSortFieldTagInfo.TagGroupValues.Add(SortField.Value.ToString());
	}
	return InternalManualSortFieldTagInfo;
}();

const FModioModTagInfo EnabledDisabledTagInfo = []() {
	FModioModTagInfo InternalEnabledDisabledTagInfo;
	InternalEnabledDisabledTagInfo.bAllowMultipleSelection = false;
	InternalEnabledDisabledTagInfo.TagGroupName = NSLOCTEXT("Modio", "EnabledDisabledTagGroupName", "Enabled/Disabled").ToString();
	for (auto& SortField : EnabledFilterFieldMap)
	{
		InternalEnabledDisabledTagInfo.TagGroupValues.Add(SortField.Value.ToString());
	}
	return InternalEnabledDisabledTagInfo;
}();

const FModioModTagInfo InstalledByCurrentAndAnotherUserTagInfo = []() {
	FModioModTagInfo InternalInstalledByCurrentAndAnotherUserTagInfo;
	InternalInstalledByCurrentAndAnotherUserTagInfo.bAllowMultipleSelection = false;
	InternalInstalledByCurrentAndAnotherUserTagInfo.TagGroupName = NSLOCTEXT("Modio", "InstalledByCurrentAndAnotherUserTagGroupName", "Installed By").ToString();
	for (auto& SortField : InstalledFilterFieldMap)
	{
		InternalInstalledByCurrentAndAnotherUserTagInfo.TagGroupValues.Add(SortField.Value.ToString());
	}
	return InternalInstalledByCurrentAndAnotherUserTagInfo;
}();

UModioCommonFilteringView::UModioCommonFilteringView()
{
	bAutoFocusOnActivation = false;
}

bool UModioCommonFilteringView::SetSelectedTagGroupValues_Implementation(const TArray<FString>& TagGroupValues,	bool bSelect)
{
	if (FilteringTagsContainer)
	{
		for (UWidget* ChildWidget : FilteringTagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagGroupList* ModTagGroupList = Cast<UModioCommonModTagGroupList>(ChildWidget))
			{
				ModTagGroupList->SetSelectedTagGroupValues(TagGroupValues, bSelect);
			}
		}
		return true;
	}
	return false;
}

bool UModioCommonFilteringView::GetSelectedTagGroupValues_Implementation(TArray<FString>& OutSelectedTagGroupValues) const
{
	if (FilteringTagsContainer)
	{
		for (UWidget* ChildWidget : FilteringTagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagGroupList* ModTagGroupList = Cast<UModioCommonModTagGroupList>(ChildWidget))
			{
				ModTagGroupList->GetSelectedTagGroupValues(OutSelectedTagGroupValues);
			}
		}
		return true;
	}
	return false;
}

FModioModCategoryParams UModioCommonFilteringView::GetFilterParamsWrapper_Implementation() const
{
	UModioSearchResultsParamsUI* SearchResultsParamsUI = Cast<UModioSearchResultsParamsUI>(DataSource);
	if (!SearchResultsParamsUI)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to get category params from data source in '%s'"), *GetName());
		return FModioModCategoryParams();
	}

	FModioModCategoryParams FilterParams;
	TArray<FString> SelectedTagGroupValues;
	GetSelectedTagGroupValues(SelectedTagGroupValues);
	TSet<FString> SelectedTagGroupValuesSet(SelectedTagGroupValues);

	if (SearchResultsParamsUI->SearchType == EModioCommonSearchViewType::SearchResults)
	{
		for (auto& SortFieldAndDirection : SortFieldAndSortDirectionMap)
		{
			if (SelectedTagGroupValuesSet.Remove(SortFieldAndDirection.Value.ToString()) > 0)
			{
				FilterParams.SortField = SortFieldAndDirection.Key.Key;
				FilterParams.Direction = SortFieldAndDirection.Key.Value;
				break;
			}
		}
	}
	else if (SearchResultsParamsUI->SearchType == EModioCommonSearchViewType::Collection)
	{
		if (SelectedTagGroupValuesSet.Remove(EnabledFilterFieldMap.FindChecked(EModioEnabledFilterType::Enabled).ToString()) > 0)
		{
			FilterParams.EnabledFilter = EModioEnabledFilterType::Enabled;
		}
		else if (SelectedTagGroupValuesSet.Remove(EnabledFilterFieldMap.FindChecked(EModioEnabledFilterType::Disabled).ToString()) > 0)
		{
			FilterParams.EnabledFilter = EModioEnabledFilterType::Disabled;
		}

		if (SelectedTagGroupValuesSet.Remove(InstalledFilterFieldMap.FindChecked(EModioInstalledFilterType::CurrentUser).ToString()) > 0)
		{
			FilterParams.InstalledField = EModioInstalledFilterType::CurrentUser;
		}
		else if (SelectedTagGroupValuesSet.Remove(InstalledFilterFieldMap.FindChecked(EModioInstalledFilterType::AnotherUser).ToString()) > 0)
		{
			FilterParams.InstalledField = EModioInstalledFilterType::AnotherUser;
		}

		for (auto& SortField : ManualSortFieldMap)
		{
			if (SelectedTagGroupValuesSet.Remove(SortField.Value.ToString()) > 0)
			{
				FilterParams.ManualSortField = SortField.Key;
				break;
			}
		}
	}

	FilterParams.Tags = SelectedTagGroupValuesSet.Array();
	return FilterParams;
}

void UModioCommonFilteringView::ResetFiltering_Implementation()
{
	if (FilteringTagsContainer)
	{
		for (UWidget* ChildWidget : FilteringTagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagGroupList* ModTagGroupList = Cast<UModioCommonModTagGroupList>(ChildWidget))
			{
				ModTagGroupList->ResetTagsSelection();
			}
		}
	}
}

void UModioCommonFilteringView::AddModTagInfo_Implementation(const FModioModTagInfo& ModTagInfo)
{
	if (FilteringTagsContainer && TagGroupListClass)
	{
		UModioCommonModTagGroupList* ModTagGroupList = CreateWidget<UModioCommonModTagGroupList>(FilteringTagsContainer.Get(), TagGroupListClass);
		ModTagGroupList->SetTagsGroup(ModTagInfo);
		FilteringTagsContainer->AddChild(ModTagGroupList);
	}
}

void UModioCommonFilteringView::SynchronizeFilterParams_Implementation(const TArray<FString>& PreviouslySelectedTagGroupValues)
{
	UModioSearchResultsParamsUI* SearchResultsParamsUI = Cast<UModioSearchResultsParamsUI>(DataSource);
	if (!SearchResultsParamsUI)
	{
		SetSelectedTagGroupValues(PreviouslySelectedTagGroupValues, true);
		return;
	}
	
	SetSelectedTagGroupValues(SearchResultsParamsUI->CurrentFilterParams.Tags, true);

	if (SearchResultsParamsUI->SearchType == EModioCommonSearchViewType::SearchResults)
	{
		if (const FText* FoundTag = SortFieldAndSortDirectionMap.Find({SearchResultsParamsUI->CurrentFilterParams.SortField, SearchResultsParamsUI->CurrentFilterParams.Direction}))
		{
			SetSelectedTagGroupValues({FoundTag->ToString()}, true);
		}
	}
	else if (SearchResultsParamsUI->SearchType == EModioCommonSearchViewType::Collection)
	{
		if (SearchResultsParamsUI->CurrentFilterParams.EnabledFilter != EModioEnabledFilterType::None)
		{
			SetSelectedTagGroupValues({EnabledFilterFieldMap[SearchResultsParamsUI->CurrentFilterParams.EnabledFilter].ToString()}, true);
		}

		if (SearchResultsParamsUI->CurrentFilterParams.InstalledField != EModioInstalledFilterType::None)
		{
			SetSelectedTagGroupValues({InstalledFilterFieldMap[SearchResultsParamsUI->CurrentFilterParams.InstalledField].ToString()}, true);
		}

		SetSelectedTagGroupValues({ManualSortFieldMap[SearchResultsParamsUI->CurrentFilterParams.ManualSortField].ToString()}, true);
	}
}

UWidget* UModioCommonFilteringView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = Super::NativeGetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (FilteringTagsContainer)
	{
		for (UWidget* ChildWidget : FilteringTagsContainer->GetAllChildren())
		{
			if (UModioCommonModTagGroupList* ModTagGroupList = Cast<UModioCommonModTagGroupList>(ChildWidget))
			{
				if (UWidget* WidgetToFocus = ModTagGroupList->GetDesiredFocusTarget())
				{
					return WidgetToFocus;
				}
			}
		}
	}
	return nullptr;
}

void UModioCommonFilteringView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	TArray<FString> SelectedTagGroupValues;
	GetSelectedTagGroupValues(SelectedTagGroupValues);

#if WITH_EDITOR
	if (IsDesignTime())
	{
		if (FilteringTagsContainer)
		{
			FilteringTagsContainer->ClearChildren();
		}
		for (const FModioModTagInfo& TagInfo : PreviewModTagInfoOptions)
		{
			AddModTagInfo(TagInfo);
		}
		return;
	}
#endif

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModTagOptionsAsync(
			FOnGetModTagOptionsDelegateFast::CreateWeakLambda(this, [SelectedTagGroupValues = MoveTemp(SelectedTagGroupValues), this](FModioErrorCode ErrorCode, TOptional<FModioModTagOptions> Options)
			{
				UModioSearchResultsParamsUI* SearchResultsParamsUI = Cast<UModioSearchResultsParamsUI>(DataSource);
				if (!SearchResultsParamsUI)
				{
					UE_LOG(ModioUI5, Error, TEXT("Unable to get search results params UI in '%s'"), *GetName());
					return;
				}

				if (FilteringTagsContainer)
				{
					FilteringTagsContainer->ClearChildren();
				}

				if (SearchResultsParamsUI->SearchType == EModioCommonSearchViewType::SearchResults)
				{
					AddModTagInfo(SortFieldAndSortDirectionTagInfo);
				}
				else if (SearchResultsParamsUI->SearchType == EModioCommonSearchViewType::Collection)
				{
					AddModTagInfo(ManualSortFieldTagInfo);
					AddModTagInfo(EnabledDisabledTagInfo);
					AddModTagInfo(InstalledByCurrentAndAnotherUserTagInfo);
				}
				if (!ErrorCode && Options.IsSet())
				{
					for (const FModioModTagInfo& TagInfo : Options.GetValue().GetRawList())
					{
						AddModTagInfo(TagInfo);
					}
					SynchronizeFilterParams(SelectedTagGroupValues);
				}
			}));
	}
}
