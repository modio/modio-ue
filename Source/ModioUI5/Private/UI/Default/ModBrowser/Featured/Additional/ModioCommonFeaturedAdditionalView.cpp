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
#include "UI/Foundation/Components/List/ModioCommonModTileView.h"
#include "UI/Settings/Params/ModioCommonModBrowserParams.h"
#include "Algo/Find.h"
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
	if (ModList)
	{
		return ModList->GetNumItemsSelected() > 0;
	}
	return false;
}

void UModioCommonFeaturedAdditionalView::ClearModSelection_Implementation()
{
	if (ModList && ModList->Implements<UModioCommonModListViewInterface>())
	{
		IModioCommonModListViewInterface::Execute_RequestFullClearSelection(ModList);
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

			if (ModList)
			{
				if (UModioCommonListView* ModListView = Cast<UModioCommonListView>(ModList))
				{
					ModListView->SetStyle(StyleCDO->ModListStyle);
				}
				else if (UModioCommonModTileView* ModTileView = Cast<UModioCommonModTileView>(ModList))
				{
					ModTileView->SetStyle(StyleCDO->ModListStyle);
				}
			}
		}

		if (ModList && ModList->GetNumItems() == 0 && Settings->AdditionalCategoryParams.Num() > 0)
		{
			RefreshList(Settings->AdditionalCategoryParams[0].ToFilterParams());
		}
	}
}

UWidget* UModioCommonFeaturedAdditionalView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (ModList)
	{
		return ModList;
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonFeaturedAdditionalView::RefreshList(const FModioFilterParams& Filter)
{
	if (!ModList)
	{
		return;
	}

#if WITH_EDITOR
	if (!IsDesignTime())
#endif
	{
		const bool bFocusOnceListIsPopulated = ModList->GetSelectedItem<UModioModInfoUI>() != nullptr;
		IModioCommonModListViewInterface::Execute_SetFocusOnceListIsPopulated(ModList, bFocusOnceListIsPopulated);
		IModioCommonModListViewInterface::Execute_RequestFullClearSelection(ModList);
		ModList->ClearListItems();
		
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem && CurrentFilter.ToString() != Filter.ToString())
		{
			CurrentFilter = Filter;
			Subsystem->RequestListAllMods(Filter, GetRequestIdentifier());
		}
	}
}

void UModioCommonFeaturedAdditionalView::NativeConstruct()
{
	Super::NativeConstruct();
	SynchronizeProperties();
}

void UModioCommonFeaturedAdditionalView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	IModioUIModInfoReceiver::Register<UModioCommonFeaturedAdditionalView>(EModioUIModInfoEventType::ListAllMods);
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

			const UModioCommonFeaturedAdditionalParamsSettings* Settings = GetDefault<UModioCommonFeaturedAdditionalParamsSettings>();
			if (!Settings)
			{
				return;
			}

			const FModioCommonFeaturedCategoryParams* SelectedAdditionalCategoryParam = Algo::FindByPredicate(Settings->AdditionalCategoryParams,
				[this, TabId](const FModioCommonFeaturedCategoryParams& Params) {
					return TabId == FName(*Params.CategoryName.ToString());
				});

			if (SelectedAdditionalCategoryParam)
			{
				RefreshList(SelectedAdditionalCategoryParam->ToFilterParams());
			}
		});
	}

	if (ModList)
	{
		ModList->OnItemSelectionChanged().AddWeakLambda(this, [this](UObject* Item) {
			if (!ModList)
			{
				return;
			}
			
			UModioModInfoUI* ModInfo = ModList->GetSelectedItem<UModioModInfoUI>();
			if (ModInfo && ModInfo == Item)
			{
				ModList->RequestScrollItemIntoView(ModInfo);
			}
		});
	}
}

void UModioCommonFeaturedAdditionalView::NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode, TOptional<FModioModInfoList> List)
{
	IModioUIModInfoReceiver::NativeOnListAllModsRequestCompleted(RequestIdentifier, ErrorCode, List);
	if (RequestIdentifier == GetRequestIdentifier())
	{
		if (ErrorCode)
		{
			UE_LOG(ModioUI5, Error, TEXT("Unable set mod list in '%s' due to error '%s'"), *GetName(), *ErrorCode.GetErrorMessage());
		}
		if (List.IsSet() && ModList && ModList->Implements<UModioCommonModListViewInterface>())
		{
			IModioCommonModListViewInterface::Execute_SetModsFromModInfoList(ModList, List.GetValue(), false);
		}
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
