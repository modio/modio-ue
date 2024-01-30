/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/ModBrowser/Featured/ModioCommonFeaturedView.h"

#include "ModioUI.h"
#include "Core/ModioFilterParamsUI.h"
#include "UI/Default/ModBrowser/Featured/ModioCommonFeaturedViewStyle.h"
#include "UI/Foundation/Components/List/ModioCommonFilteredModListView.h"
#include "UI/Foundation/Components/Tab/ModioCommonTabListWidgetBase.h"
#include "UI/Settings/ModioCommonUISettings.h"
#include "UI/Settings/Params/ModioCommonModBrowserParams.h"
#include "Algo/Find.h"
#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"
#include "UI/Default/Search/ModioCommonSearchResultsView.h"

UModioCommonFeaturedView::UModioCommonFeaturedView()
{
	//bAutoFocusOnActivation = false;
}

void UModioCommonFeaturedView::SetStyle(TSubclassOf<UModioCommonFeaturedViewStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonFeaturedView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonFeaturedViewStyle* StyleCDO = Cast<UModioCommonFeaturedViewStyle>(ModioStyle.GetDefaultObject()))
	{
		if (SearchResultsView)
		{
			SearchResultsView->SetStyle(StyleCDO->SearchResultsViewStyle);
		}
	}
}

void UModioCommonFeaturedView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	UModioFilterParamsUI* FilterParamsPtr = Cast<UModioFilterParamsUI>(DataSource);
	if (!FilterParamsPtr)
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to set data source for '%s': DataSource is not of type UModioFilterParamsUI"), *GetName());
		return;
	}

	FModioFilterParams& FilterParams = FilterParamsPtr->Underlying;

	if (bWasFilterAppliedAtLeastOnce)
	{
		if (PreviouslyAppliedFilter.ToString() == FilterParams.ToString() && FilterParams.Count == PreviouslyAppliedFilter.Count == OverriddenModsCount)
		{
			UE_LOG(ModioUI, Warning, TEXT("Filter is the same as the current filter, skipping refresh"));
			return;
		}
	}

	if (OverriddenModsCount > 0)
	{
		FilterParams.Count = OverriddenModsCount;
	}
	PreviouslyAppliedFilter = FilterParams;

	SynchronizeProperties();

	if (SearchResultsView)
	{
		SearchResultsView->SetDataSource(FilterParamsPtr);
	}
	else
	{
		UE_LOG(ModioUI, Error, TEXT("Unable to set data source for '%s': FilteredModListView is not bound"), *GetName());
	}

	bWasFilterAppliedAtLeastOnce = true;
}

UWidget* UModioCommonFeaturedView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (SearchResultsView)
	{
		if (UWidget* WidgetToFocus = SearchResultsView->GetDesiredFocusTarget())
		{
			return WidgetToFocus;
		}
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonFeaturedView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	IModioUIUserChangedReceiver::Register<UModioCommonFeaturedView>();
	IModioUISubscriptionsChangedReceiver::Register<UModioCommonFeaturedView>();
	IModioUIModManagementEventReceiver::Register<UModioCommonFeaturedView>();
}

void UModioCommonFeaturedView::RefreshList_Implementation()
{
	if (UModioFilterParamsUI* FilterParamsUI = Cast<UModioFilterParamsUI>(DataSource))
	{
		SetDataSource(FilterParamsUI);
	}
}

void UModioCommonFeaturedView::RefreshListByFilter_Implementation(const FModioFilterParams& Filter)
{
	if (UModioFilterParamsUI* FilterParamsUI = Cast<UModioFilterParamsUI>(DataSource))
	{
		FilterParamsUI->Underlying = Filter;
		SetDataSource(FilterParamsUI);
	}
}

void UModioCommonFeaturedView::RefreshListByCategoryName_Implementation(FName InCategoryName)
{
	if (!SearchResultsView)
	{
		return;
	}

#if WITH_EDITOR
	if (!IsDesignTime())
#endif
	{
		const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>();
		if (!UISettings)
		{
			return;
		}

		const FModioModCategoryParams* SelectedCategoryParamsPtr = Algo::FindByPredicate(UISettings->FeaturedParams.CategoryParams,
			[this, InCategoryName](const FModioModCategoryParams& Params) {
				return InCategoryName == FName(*Params.CategoryName.ToString());
			});

		if (SelectedCategoryParamsPtr)
		{
			FModioModCategoryParams SelectedCategoryParams = *SelectedCategoryParamsPtr;
			RefreshListByFilter(SelectedCategoryParams.ToFilterParams());
		}
		else
		{
			UE_LOG(ModioUI, Error, TEXT("Unable to find category '%s' in settings"), *InCategoryName.ToString());
		}
	}
}