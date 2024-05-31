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
#include "ModioUI.h"
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

const TMap<EModioQueuedFilterType, FText> QueuedFieldMap = []() {
	TMap<EModioQueuedFilterType, FText> InternalQueuedFieldMap;
	InternalQueuedFieldMap.Add(EModioQueuedFilterType::Queued, NSLOCTEXT("Modio", "Queued", "Queued"));
	InternalQueuedFieldMap.Add(EModioQueuedFilterType::NotQueued, NSLOCTEXT("Modio", "NotQueued", "Not Queued"));
	return InternalQueuedFieldMap;
}();

const FModioModTagInfo SortFieldAndSortDirectionTagInfo = []() {
	FModioModTagInfo InternalSortFieldAndSortDirectionTagInfo;
	InternalSortFieldAndSortDirectionTagInfo.bAllowMultipleSelection = false;
	InternalSortFieldAndSortDirectionTagInfo.TagGroupName = NSLOCTEXT("Modio", "SortFieldAndSortDirectionTagGroupName", "Sort by").ToString();
	InternalSortFieldAndSortDirectionTagInfo.LocalizedTagCategoryDataCurrentLocale.GroupName = InternalSortFieldAndSortDirectionTagInfo.TagGroupName;
	InternalSortFieldAndSortDirectionTagInfo.TagGroupValues.Reserve(SortFieldAndSortDirectionMap.Num());
	for (auto& SortFieldAndDirection : SortFieldAndSortDirectionMap)
	{
		InternalSortFieldAndSortDirectionTagInfo.TagGroupValues.Add(SortFieldAndDirection.Value.ToString());
	}
	InternalSortFieldAndSortDirectionTagInfo.LocalizedTagCategoryDataCurrentLocale.Tags = InternalSortFieldAndSortDirectionTagInfo.TagGroupValues;
	return InternalSortFieldAndSortDirectionTagInfo;
}();

const FModioModTagInfo ManualSortFieldTagInfo = []() {
	FModioModTagInfo InternalManualSortFieldTagInfo;
	InternalManualSortFieldTagInfo.bAllowMultipleSelection = false;
	InternalManualSortFieldTagInfo.TagGroupName = NSLOCTEXT("Modio", "ManualSortFieldTagGroupName", "Sort by").ToString();
	InternalManualSortFieldTagInfo.LocalizedTagCategoryDataCurrentLocale.GroupName = InternalManualSortFieldTagInfo.TagGroupName;
	InternalManualSortFieldTagInfo.TagGroupValues.Reserve(ManualSortFieldMap.Num());
	for (auto& SortField : ManualSortFieldMap)
	{
		InternalManualSortFieldTagInfo.TagGroupValues.Add(SortField.Value.ToString());
	}
	InternalManualSortFieldTagInfo.LocalizedTagCategoryDataCurrentLocale.Tags = InternalManualSortFieldTagInfo.TagGroupValues;
	return InternalManualSortFieldTagInfo;
}();

const FModioModTagInfo EnabledDisabledTagInfo = []() {
	FModioModTagInfo InternalEnabledDisabledTagInfo;
	InternalEnabledDisabledTagInfo.bAllowMultipleSelection = false;
	InternalEnabledDisabledTagInfo.TagGroupName = NSLOCTEXT("Modio", "EnabledDisabledTagGroupName", "Enabled/Disabled").ToString();
	InternalEnabledDisabledTagInfo.LocalizedTagCategoryDataCurrentLocale.GroupName = InternalEnabledDisabledTagInfo.TagGroupName;
	InternalEnabledDisabledTagInfo.TagGroupValues.Reserve(EnabledFilterFieldMap.Num());
	for (auto& SortField : EnabledFilterFieldMap)
	{
		InternalEnabledDisabledTagInfo.TagGroupValues.Add(SortField.Value.ToString());
	}
	InternalEnabledDisabledTagInfo.LocalizedTagCategoryDataCurrentLocale.Tags = InternalEnabledDisabledTagInfo.TagGroupValues;
	return InternalEnabledDisabledTagInfo;
}();

const FModioModTagInfo InstalledByCurrentAndAnotherUserTagInfo = []() {
	FModioModTagInfo InternalInstalledByCurrentAndAnotherUserTagInfo;
	InternalInstalledByCurrentAndAnotherUserTagInfo.bAllowMultipleSelection = false;
	InternalInstalledByCurrentAndAnotherUserTagInfo.TagGroupName = NSLOCTEXT("Modio", "InstalledByCurrentAndAnotherUserTagGroupName", "Installed By").ToString();
	InternalInstalledByCurrentAndAnotherUserTagInfo.LocalizedTagCategoryDataCurrentLocale.GroupName = InternalInstalledByCurrentAndAnotherUserTagInfo.TagGroupName;
	InternalInstalledByCurrentAndAnotherUserTagInfo.TagGroupValues.Reserve(InstalledFilterFieldMap.Num());
	for (auto& SortField : InstalledFilterFieldMap)
	{
		InternalInstalledByCurrentAndAnotherUserTagInfo.TagGroupValues.Add(SortField.Value.ToString());
	}
	InternalInstalledByCurrentAndAnotherUserTagInfo.LocalizedTagCategoryDataCurrentLocale.Tags = InternalInstalledByCurrentAndAnotherUserTagInfo.TagGroupValues;
	return InternalInstalledByCurrentAndAnotherUserTagInfo;
}();

const FModioModTagInfo QueuedTagInfo = []() {
	FModioModTagInfo InternalQueuedTagInfo;
	InternalQueuedTagInfo.bAllowMultipleSelection = false;
	InternalQueuedTagInfo.TagGroupName = NSLOCTEXT("Modio", "QueuedTagGroupName", "Queued").ToString();
	InternalQueuedTagInfo.LocalizedTagCategoryDataCurrentLocale.GroupName = InternalQueuedTagInfo.TagGroupName;
	InternalQueuedTagInfo.TagGroupValues.Reserve(QueuedFieldMap.Num());
	for (auto& QueuedField : QueuedFieldMap)
	{
		InternalQueuedTagInfo.TagGroupValues.Add(QueuedField.Value.ToString());
	}
	InternalQueuedTagInfo.LocalizedTagCategoryDataCurrentLocale.Tags = InternalQueuedTagInfo.TagGroupValues;
	return InternalQueuedTagInfo;
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
		UE_LOG(ModioUI, Error, TEXT("Unable to get category params from data source in '%s'"), *GetName());
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

		if (SelectedTagGroupValuesSet.Remove(QueuedFieldMap.FindChecked(EModioQueuedFilterType::Queued).ToString()) > 0)
		{
			FilterParams.QueuedField = EModioQueuedFilterType::Queued;
		}
		else if (SelectedTagGroupValuesSet.Remove(QueuedFieldMap.FindChecked(EModioQueuedFilterType::NotQueued).ToString()) > 0)
		{
			FilterParams.QueuedField = EModioQueuedFilterType::NotQueued;
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
	if (MaxNumOfTags > 0 && FilterParams.Tags.Num() > MaxNumOfTags)
	{
		FilterParams.Tags.RemoveAt(0, FilterParams.Tags.Num() - MaxNumOfTags);
	}
	return FilterParams;
}

void UModioCommonFilteringView::ResetFiltering_Implementation()
{
	TArray<FString> SelectedTagGroupValues;
	GetSelectedTagGroupValues(SelectedTagGroupValues);
	SynchronizeFilterParams(SelectedTagGroupValues, true);
}

void UModioCommonFilteringView::ZeroOutFiltering_Implementation()
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

void UModioCommonFilteringView::SynchronizeFilterParams_Implementation(const TArray<FString>& PreviouslySelectedTagGroupValues, bool bResetToDefault)
{
	ZeroOutFiltering();
	
	UModioSearchResultsParamsUI* SearchResultsParamsUI = Cast<UModioSearchResultsParamsUI>(DataSource);
	if (!SearchResultsParamsUI)
	{
		SetSelectedTagGroupValues(PreviouslySelectedTagGroupValues, true);
		return;
	}

	const FModioModCategoryParams& FilterParams = bResetToDefault ? SearchResultsParamsUI->DefaultFilterParams : SearchResultsParamsUI->FilterParams;
	
	SetSelectedTagGroupValues(FilterParams.Tags, true);

	if (SearchResultsParamsUI->SearchType == EModioCommonSearchViewType::SearchResults)
	{
		if (const FText* FoundTag = SortFieldAndSortDirectionMap.Find({FilterParams.SortField, FilterParams.Direction}))
		{
			SetSelectedTagGroupValues({FoundTag->ToString()}, true);
		}
	}
	else if (SearchResultsParamsUI->SearchType == EModioCommonSearchViewType::Collection)
	{
		if (FilterParams.EnabledFilter != EModioEnabledFilterType::None)
		{
			SetSelectedTagGroupValues({EnabledFilterFieldMap[FilterParams.EnabledFilter].ToString()}, true);
		}

		if (FilterParams.InstalledField != EModioInstalledFilterType::None)
		{
			SetSelectedTagGroupValues({InstalledFilterFieldMap[FilterParams.InstalledField].ToString()}, true);
		}

		if (FilterParams.QueuedField != EModioQueuedFilterType::None)
		{
			SetSelectedTagGroupValues({QueuedFieldMap[FilterParams.QueuedField].ToString()}, true);
		}

		SetSelectedTagGroupValues({ManualSortFieldMap[FilterParams.ManualSortField].ToString()}, true);
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
					UE_LOG(ModioUI, Error, TEXT("Unable to get search results params UI in '%s'"), *GetName());
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
					if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
					{
						if (Subsystem->GetIsCollectionModDisableUIEnabled())
						{
							AddModTagInfo(EnabledDisabledTagInfo);
							UE_LOG(ModioUI, Warning, TEXT("Added enabled/disabled tag info to '%s' since collection mod disable UI is enabled"), *GetName());
						}
						else
						{
							UE_LOG(ModioUI, Warning, TEXT("Did not add enabled/disabled tag info to '%s' since collection mod disable UI is disabled"), *GetName());
						}
					}
					else
					{
						UE_LOG(ModioUI, Error, TEXT("Unable to add enabled/disabled tag info to '%s' since modio UI subsystem is not available"), *GetName());
					}
					AddModTagInfo(InstalledByCurrentAndAnotherUserTagInfo);
					AddModTagInfo(QueuedTagInfo);
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
