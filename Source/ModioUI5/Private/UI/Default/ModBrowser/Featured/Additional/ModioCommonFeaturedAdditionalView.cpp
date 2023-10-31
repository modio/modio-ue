/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/ModBrowser/Featured/Additional/ModioCommonFeaturedAdditionalView.h"

#include "ModioUI5.h"
#include "Core/ModioModInfoUI.h"
#include "UI/Default/ModBrowser/Featured/ModioCommonFeaturedViewStyle.h"
#include "UI/Foundation/Components/Tab/ModioCommonTabButtonBase.h"
#include "UI/Foundation/Components/Tab/ModioCommonTabListWidgetBase.h"
#include "UI/Settings/Params/ModioCommonModBrowserParams.h"
#include "Algo/Find.h"
#include "Core/ModioFilterParamsUI.h"
#include "UI/Foundation/Components/List/ModioCommonFilteredModListView.h"
#include "UI/Foundation/Components/List/ModioCommonListView.h"

UModioCommonFeaturedAdditionalView::UModioCommonFeaturedAdditionalView()
{
	bAutoFocusOnActivation = false;
}

void UModioCommonFeaturedAdditionalView::SetStyle(TSubclassOf<UModioCommonFeaturedAdditionalViewStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

bool UModioCommonFeaturedAdditionalView::IsAnyModSelected_Implementation()
{
	if (FilteredModListView)
	{
		return FilteredModListView->GetNumItemsSelected() > 0;
	}
	return false;
}

void UModioCommonFeaturedAdditionalView::ClearModSelection_Implementation()
{
	if (FilteredModListView && FilteredModListView->Implements<UModioCommonModListViewInterface>())
	{
		IModioCommonModListViewInterface::Execute_RequestFullClearSelection(FilteredModListView);
	}
}

void UModioCommonFeaturedAdditionalView::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	if (const UModioCommonFeaturedAdditionalParamsSettings* Settings = GetDefault<UModioCommonFeaturedAdditionalParamsSettings>())
	{
		if (const UModioCommonFeaturedAdditionalViewStyle* StyleCDO = Cast<UModioCommonFeaturedAdditionalViewStyle>(ModioStyle.GetDefaultObject()))
		{
			if (CategoryTabList)
			{
				CategoryTabList->RemoveAllDynamicTabs();
				CategoryTabList->SetPreviousTabInputActionData(Settings->PreviousTabInputAction);
				CategoryTabList->SetNextTabInputActionData(Settings->NextTabInputAction);
				for (const FModioCommonFeaturedCategoryParams& Params : Settings->AdditionalCategoryParams)
				{
					FModioCommonTabDescriptor FeaturedTabInfo;
					FeaturedTabInfo.TabId = FName(*Params.CategoryName.ToString());
					FeaturedTabInfo.TabText = Params.CategoryName;
					FeaturedTabInfo.TabButtonType = StyleCDO->CategoryTabButtonClass;
					FeaturedTabInfo.TabButtonStyle = StyleCDO->CategoryTabButtonStyle;
					CategoryTabList->RegisterDynamicTab(FeaturedTabInfo);
				}
			}

			if (FilteredModListView)
			{
				FilteredModListView->SetStyle(StyleCDO->FilteredModListViewStyle);
			}
		}

		if (FilteredModListView && FilteredModListView->GetNumItems() == 0 && Settings->AdditionalCategoryParams.Num() > 0)
		{
			FModioCommonFeaturedCategoryParams AdditionalCategoryParams = Settings->AdditionalCategoryParams[0];
			if (OverriddenModsCount > 0)
			{
				AdditionalCategoryParams.Count = OverriddenModsCount;
			}
			RefreshList(AdditionalCategoryParams.ToFilterParams());
		}
	}
}

UWidget* UModioCommonFeaturedAdditionalView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (FilteredModListView)
	{
		if (UWidget* WidgetToFocus = FilteredModListView->GetDesiredFocusTarget())
		{
			return WidgetToFocus;
		}
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonFeaturedAdditionalView::RefreshList_Implementation(const FModioFilterParams& Filter)
{
	if (!FilteredModListView)
	{
		return;
	}

	if (CurrentFilter.ToString() != Filter.ToString())
	{
		UModioFilterParamsUI* FilterParamsUI = NewObject<UModioFilterParamsUI>();
		FilterParamsUI->Underlying = Filter;
		FilteredModListView->SetDataSource(FilterParamsUI);
	}
}

void UModioCommonFeaturedAdditionalView::RefreshListByTabId_Implementation(FName TabId)
{
	if (!FilteredModListView)
	{
		return;
	}

#if WITH_EDITOR
	if (!IsDesignTime())
#endif
	{
		const UModioCommonFeaturedAdditionalParamsSettings* Settings = GetDefault<UModioCommonFeaturedAdditionalParamsSettings>();
		if (!Settings)
		{
			return;
		}

		const FModioCommonFeaturedCategoryParams* SelectedAdditionalCategoryParamsPtr = Algo::FindByPredicate(Settings->AdditionalCategoryParams,
			[this, TabId](const FModioCommonFeaturedCategoryParams& Params) {
				return TabId == FName(*Params.CategoryName.ToString());
			});

		if (SelectedAdditionalCategoryParamsPtr)
		{
			FModioCommonFeaturedCategoryParams SelectedAdditionalCategoryParams = *SelectedAdditionalCategoryParamsPtr;
			if (OverriddenModsCount > 0)
			{
				SelectedAdditionalCategoryParams.Count = OverriddenModsCount;
			}
			RefreshList(SelectedAdditionalCategoryParams.ToFilterParams());
		}
	}
}

void UModioCommonFeaturedAdditionalView::HandleSetModsFromModInfoListFinished_Implementation()
{
	
}

void UModioCommonFeaturedAdditionalView::HandleSetModsFromModInfoListStarted_Implementation()
{
	
}

void UModioCommonFeaturedAdditionalView::NativeConstruct()
{
	Super::NativeConstruct();
	SynchronizeProperties();
}

void UModioCommonFeaturedAdditionalView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	IModioUIUserChangedReceiver::Register<UModioCommonFeaturedAdditionalView>();
	IModioUISubscriptionsChangedReceiver::Register<UModioCommonFeaturedAdditionalView>();
	IModioUIModManagementEventReceiver::Register<UModioCommonFeaturedAdditionalView>();

	if (CategoryTabList)
	{
		CategoryTabList->OnTabSelectedFast.AddWeakLambda(this, [this](FName TabId) {
			if (!IsActivated())
			{
				return;
			}
			RefreshListByTabId(TabId);
		});
	}

	if (FilteredModListView)
	{
		FilteredModListView->OnSetModsFromModInfoListStarted.AddWeakLambda(this, [this]() {
			if (!IsActivated())
			{
				return;
			}
			HandleSetModsFromModInfoListStarted();
		});
		FilteredModListView->OnSetModsFromModInfoListFinished.AddWeakLambda(this, [this]() {
			if (!IsActivated())
			{
				return;
			}
			HandleSetModsFromModInfoListFinished();
		});
	}
}

void UModioCommonFeaturedAdditionalView::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (CategoryTabList)
	{
		CategoryTabList->SetListeningForInput(true);
	}
}

void UModioCommonFeaturedAdditionalView::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (CategoryTabList)
	{
		CategoryTabList->SetListeningForInput(false);
	}
}

void UModioCommonFeaturedAdditionalView::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	if (CategoryTabList)
	{
		CategoryTabList->SetListeningForInput(true);
	}
}

void UModioCommonFeaturedAdditionalView::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (CategoryTabList)
	{
		CategoryTabList->SetListeningForInput(false);
	}
}
