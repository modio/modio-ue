/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/ModBrowser/Featured/Primary/ModioCommonFeaturedPrimaryView.h"

#include "ModioUI5.h"
#include "Core/ModioModInfoUI.h"
#include "UI/Default/ModBrowser/Featured/ModioCommonFeaturedViewStyle.h"
#include "UI/Default/ModBrowser/Featured/Primary/ModioCommonFeaturedPrimaryLargeModEntry.h"
#include "UI/Foundation/Components/List/ModioCommonModListView.h"
#include "UI/Foundation/Components/List/ModioCommonModTileView.h"
#include "UI/Settings/Params/ModioCommonModBrowserParams.h"

UModioCommonFeaturedPrimaryView::UModioCommonFeaturedPrimaryView()
{
	bAutoFocusOnActivation = false;
}

void UModioCommonFeaturedPrimaryView::SetStyle(TSubclassOf<UModioCommonFeaturedPrimaryViewStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

void UModioCommonFeaturedPrimaryView::FetchPrimaryCategoryMods_Implementation()
{
	if (const UModioCommonFeaturedPrimaryParamsSettings* Settings = GetDefault<UModioCommonFeaturedPrimaryParamsSettings>())
	{
		FModioFilterParams Filter = FModioFilterParams()
									.WithTags(Settings->PrimaryCategoryParam.Tags)
									.WithoutTags(Settings->PrimaryCategoryParam.ExcludedTags)
									.SortBy(Settings->PrimaryCategoryParam.SortField,
											Settings->PrimaryCategoryParam.Direction)
									.IndexedResults(0, Settings->PrimaryCategoryParam.Count);
		DisplayModsMatchingFilter(Filter);
	}
	else
	{
		DisplayModsMatchingFilter(FModioFilterParams().IndexedResults(0, 5));
	}
}

void UModioCommonFeaturedPrimaryView::DisplayModsMatchingFilter_Implementation(const FModioFilterParams& Filter)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (Subsystem && CurrentFilter.ToString() != Filter.ToString())
		{
			CurrentFilter = Filter;
			Subsystem->RequestListAllMods(Filter, GetRequestIdentifier());
		}
	}
}

bool UModioCommonFeaturedPrimaryView::IsAnyModSelected_Implementation()
{
	if (ModList)
	{
		return ModList->GetNumItemsSelected() > 0;
	}
	return false;
}

void UModioCommonFeaturedPrimaryView::ClearModSelection_Implementation()
{
	if (ModList && ModList->Implements<UModioCommonModListViewInterface>())
	{
		if (UObject* SelectedItem = ModList->GetSelectedItem<UObject>())
		{
			if (UModioCommonModEntryBase* SelectedModEntry = ModList->GetEntryWidgetFromItem<UModioCommonModEntryBase>(SelectedItem))
			{
				SelectedModEntry->UnbindInputActions();
			}
		}
	}
}

void UModioCommonFeaturedPrimaryView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonFeaturedPrimaryViewStyle* StyleCDO = Cast<UModioCommonFeaturedPrimaryViewStyle>(ModioStyle.GetDefaultObject()))
	{
		if (LargeModEntry)
		{
			LargeModEntry->SetStyle(StyleCDO->LargeModEntryStyle);
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
	
	FetchPrimaryCategoryMods();
}

void UModioCommonFeaturedPrimaryView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	IModioUIModInfoReceiver::Register<UModioCommonFeaturedPrimaryView>(EModioUIModInfoEventType::ListAllMods);
	IModioUIUserChangedReceiver::Register<UModioCommonFeaturedPrimaryView>();
	IModioUISubscriptionsChangedReceiver::Register<UModioCommonFeaturedPrimaryView>();
	IModioUIModManagementEventReceiver::Register<UModioCommonFeaturedPrimaryView>();

	if (LargeModEntry)
	{
		LargeModEntry->OnFocusChangedFast.AddWeakLambda(this, [this](bool bHasFocus) {
			if (ModList && bHasFocus)
			{
				ModList->SetFocus();
			}
		});
		LargeModEntry->OnLargeModEntryMouseChanged.AddWeakLambda(this, [this](bool bReceived) {
			if (ModList && ModList->Implements<UModioCommonModListViewInterface>())
			{
				if (bReceived)
				{
					if (LargeModEntry && LargeModEntry->Implements<UModioModInfoUIDetails>())
					{
						const FModioModID ModID = IModioModInfoUIDetails::Execute_GetModID(LargeModEntry);
						IModioCommonModListViewInterface::Execute_SetModSelectionByID(ModList, ModID);
					}
				}
				else
				{
					IModioCommonModListViewInterface::Execute_RequestFullClearSelection(ModList);
				}
			}
		});
	}

	if (ModList)
	{
		ModList->OnItemSelectionChanged().AddWeakLambda(this, [this](UObject* Item)
		{
			if (!ModList)
			{
				return;
			}
			
			UObject* SelectedItem = ModList->GetSelectedItem<UObject>();

			if (LargeModEntry)
			{
				if (SelectedItem && SelectedItem == Item)
				{
					LargeModEntry->SetDataSource(SelectedItem);
					ModList->RequestScrollItemIntoView(SelectedItem);
				}

				LargeModEntry->UpdateItemSelection(*LargeModEntry, SelectedItem != nullptr);
			}
		});
	}
}

UWidget* UModioCommonFeaturedPrimaryView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (ModList)
	{
		if (LargeModEntry && LargeModEntry->Implements<UModioModInfoUIDetails>())
		{
			const FModioModID ModID = IModioModInfoUIDetails::Execute_GetModID(LargeModEntry);
			IModioCommonModListViewInterface::Execute_SetModSelectionByID(ModList, ModID);
		}
		if (ModList->Implements<UModioCommonModListViewInterface>())
		{
			return IModioCommonModListViewInterface::Execute_GetDesiredListFocusTarget(ModList);
		}
		return ModList;
	}
	return LargeModEntry;
}

void UModioCommonFeaturedPrimaryView::NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode, TOptional<FModioModInfoList> List)
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

			if (LargeModEntry && LargeModEntry->IsModListItemSelected())
			{
				ModList->SetSelectedIndex(0);
			}
			else
			{
				if (UObject* SelectedItem = ModList->GetSelectedItem<UObject>())
				{
					LargeModEntry->SetDataSource(SelectedItem);
				}
				else if (UObject* FirstItem = ModList->GetItemAt(0))
				{
					LargeModEntry->SetDataSource(FirstItem);
				}
			}
		}
	}
}

void UModioCommonFeaturedPrimaryView::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	if (LargeModEntry)
	{
		LargeModEntry->UpdateItemSelection(*LargeModEntry, true);
	}
}

void UModioCommonFeaturedPrimaryView::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (LargeModEntry)
	{
		LargeModEntry->UpdateItemSelection(*LargeModEntry, false);
	}
}
