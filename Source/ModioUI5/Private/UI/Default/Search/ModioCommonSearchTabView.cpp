/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/Search/ModioCommonSearchTabView.h"

#include "ModioUI5.h"
#include "ModioUISubsystem.h"
#include "UI/Default/Search/ModioCommonFilteringView.h"
#include "UI/Default/Search/ModioCommonSearchTabViewStyle.h"
#include "UI/Default/Search/ModioCommonSortingView.h"
#include "UI/Foundation/Base/ModioCommonWidgetSwitcher.h"
#include "UI/Foundation/Components/Border/ModioCommonBorder.h"
#include "UI/Foundation/Components/Tab/ModioCommonTabListWidgetBase.h"
#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBox.h"
#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBox.h"
#include "UI/Interfaces/IModioModBrowser.h"
#include "UI/Settings/Params/ModioCommonSearchParams.h"

const FName SortingTabId = FName(TEXT("Sorting"));
const FName FilteringTabId = FName(TEXT("Filtering"));

void UModioCommonSearchTabView::SetStyle(TSubclassOf<UModioCommonSearchTabViewStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

UWidget* UModioCommonSearchTabView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (SearchButton)
	{
		return SearchButton;
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonSearchTabView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (OverlayBackgroundBorder)
	{
		OverlayBackgroundBorder->OnModioCommonBorderClicked.AddWeakLambda(this, [this]() {
			DeactivateWidget();
		});
	}

	if (const UModioCommonSearchParamsSettings* Settings = GetDefault<UModioCommonSearchParamsSettings>())
	{
		ListenForInputAction(CloseButton, Settings->CloseInputAction, Settings->CloseButtonLabel, [this]() {
			DeactivateWidget();
		});

		ListenForInputAction(SearchButton, Settings->SearchInputAction, Settings->SearchButtonLabel, [this]() {
			ShowSearchResults();
		});

		ListenForInputAction(ClearAllButton, Settings->ClearAllInputAction, Settings->ClearAllButtonLabel, [this]() {
			ClearAllFilters();
		});
	}

	if (SortingFilteringTabList)
	{
		SortingFilteringTabList->OnTabSelectedFast.AddWeakLambda(this, [this](FName TabNameID) {
			if (!IsActivated())
			{
				return;
			}

			UModioCommonActivatableWidget* SelectedView;
			if (!GetViewFromTabNameID(TabNameID, SelectedView))
			{
				UE_LOG(ModioUI5, Error, TEXT("Unable to process tab selection in '%s' for tab '%s': no associated view found"), *GetName(), *TabNameID.ToString());
				return;
			}

			SelectedTabID = TabNameID;

			if (SortingFilteringContentSwitcher)
			{
				SortingFilteringContentSwitcher->SetActiveWidget(SelectedView);
			}

			if (SortingFilteringContentSwitcher->HasFocusedDescendants())
			{
				FocusOnDesiredWidget();
			}
		});
	}

	if (SearchTextBox)
	{
		SearchTextBox->OnTextCommitted.AddDynamic(this, &UModioCommonSearchTabView::OnSearchTextCommitted);
	}
}

void UModioCommonSearchTabView::NativeConstruct()
{
	Super::NativeConstruct();

	if (SortingFilteringTabList)
	{
		SortingFilteringTabList->SetListeningForInput(true);
	}
}

void UModioCommonSearchTabView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonSearchParamsSettings* Settings = GetDefault<UModioCommonSearchParamsSettings>())
	{
		if (CloseButton)
		{
			CloseButton->SetLabel(Settings->CloseButtonLabel);
		}

		if (SearchButton)
		{
			SearchButton->SetLabel(Settings->SearchButtonLabel);
		}

		if (ClearAllButton)
		{
			ClearAllButton->SetLabel(Settings->ClearAllButtonLabel);
		}
	}

	if (UModioCommonSearchTabViewStyle* StyleCDO = Cast<UModioCommonSearchTabViewStyle>(ModioStyle.GetDefaultObject()))
	{
		if (SortingFilteringContentSwitcher
			&& ((SortingView && !SortingView->IsA(StyleCDO->SortingViewClass)) || !SortingView)
			&& ((FilteringView && !FilteringView->IsA(StyleCDO->FilteringViewClass)) || !FilteringView))
		{
			SortingFilteringContentSwitcher->ClearChildren();
			if (StyleCDO->SortingViewClass)
			{
				SortingView = CreateWidget<UModioCommonSortingView>(this, StyleCDO->SortingViewClass);
				SortingFilteringContentSwitcher->AddChild(SortingView);
			}
			
			if (StyleCDO->FilteringViewClass)
			{
				FilteringView = CreateWidget<UModioCommonFilteringView>(this, StyleCDO->FilteringViewClass);
				SortingFilteringContentSwitcher->AddChild(FilteringView);
			}

#if WITH_EDITOR
			SortingFilteringContentSwitcher->SetActiveWidget(bPreviewDisplaySortingOrFiltering ? Cast<UWidget>(SortingView) : Cast<UWidget>(FilteringView));
#endif
		}

		if (const UModioCommonSearchParamsSettings* Settings = GetDefault<UModioCommonSearchParamsSettings>())
		{
			if (SortingFilteringTabList)
			{
				SortingFilteringTabList->RemoveAllDynamicTabs();
				SortingFilteringTabList->SetPreviousTabInputActionData(Settings->PreviousTabInputAction);
				SortingFilteringTabList->SetNextTabInputActionData(Settings->NextTabInputAction);
				
				// Sorting
				if (StyleCDO->SortingTabButtonClass)
				{
					FModioCommonTabDescriptor SortingTabInfo;
					SortingTabInfo.TabId = SortingTabId;
					SortingTabInfo.TabText = Settings->SortingTabLabel;
					SortingTabInfo.TabButtonType = StyleCDO->SortingTabButtonClass;
					SortingTabInfo.TabButtonStyle = StyleCDO->SortingTabButtonStyle;
					SortingFilteringTabList->RegisterDynamicTab(SortingTabInfo);
				}

				// Filtering
				if (StyleCDO->FilteringTabButtonClass)
				{
					FModioCommonTabDescriptor FilteringTabInfo;
					FilteringTabInfo.TabId = FilteringTabId;
					FilteringTabInfo.TabText = Settings->FilteringTabLabel;
					FilteringTabInfo.TabButtonType = StyleCDO->FilteringTabButtonClass;
					FilteringTabInfo.TabButtonStyle = StyleCDO->FilteringTabButtonStyle;
					SortingFilteringTabList->RegisterDynamicTab(FilteringTabInfo);
				}

				if (!SortingFilteringTabList->SelectTabByID(SelectedTabID))
				{
					SortingFilteringTabList->SelectTabByID(SortingTabId);
				}
			}
		}
		
		if (InternalBackgroundBorder)
		{
			InternalBackgroundBorder->SetStyle(StyleCDO->InternalBackgroundBorderStyle);
		}

		if (OverlayBackgroundBorder)
		{
			OverlayBackgroundBorder->SetStyle(StyleCDO->OverlayBackgroundBorderStyle);
		}

		if (SearchTextBox)
		{
			SearchTextBox->SetStyle(StyleCDO->SearchTextBoxStyle);
		}

		if (CloseButton)
		{
			CloseButton->SetStyle(StyleCDO->CloseButtonStyle);
		}

		if (SearchButton)
		{
			SearchButton->SetStyle(StyleCDO->SearchButtonStyle);
		}

		if (ClearAllButton)
		{
			ClearAllButton->SetStyle(StyleCDO->ClearAllButtonStyle);
		}
	}
}

void UModioCommonSearchTabView::OnSearchTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::Type::OnEnter)
	{
		ShowSearchResults();
	}
}

void UModioCommonSearchTabView::ShowSearchResults_Implementation()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		DeactivateWidget();
		if (Subsystem->ModBrowserInstance->Implements<UModioModBrowserInterface>())
		{
			IModioModBrowserInterface::Execute_ShowSearchResults(Subsystem->ModBrowserInstance, GetFilterParams());
		}
	}
}

bool UModioCommonSearchTabView::GetViewFromTabNameID_Implementation(FName TabNameID, UModioCommonActivatableWidget*& OutView) const
{
	if (SortingView && TabNameID.IsEqual(SortingTabId))
	{
		OutView = SortingView;
		return true;
	}
	if (FilteringView && TabNameID.IsEqual(FilteringTabId))
	{
		OutView = FilteringView;
		return true;
	}

	UE_LOG(ModioUI5, Error, TEXT("Unable to get view from tab name ID '%s' in '%s': no associated view found"), *TabNameID.ToString(), *GetName());
	return false;
}

void UModioCommonSearchTabView::ClearAllFilters_Implementation()
{
	if (SortingView)
	{
		SortingView->ResetSorting();
	}

	if (FilteringView)
	{
		FilteringView->ResetFiltering();
	}

	if (SearchTextBox)
	{
		SearchTextBox->SetText(FText::GetEmpty());
	}
}

FModioFilterParams UModioCommonSearchTabView::GetFilterParams_Implementation() const
{
	FModioFilterParams FilterParams;

	if (SearchTextBox)
	{
		FilterParams.NameContains(SearchTextBox->GetText().ToString());
	}

	if (SortingView)
	{
		EModioSortFieldType SortFieldType;
		EModioSortDirection SortDirection;
		if (SortingView->GetSortFieldTypeAndDirection(SortFieldType, SortDirection))
		{
			FilterParams.SortBy(SortFieldType, SortDirection);
		}
	}

	if (FilteringView)
	{
		TArray<FString> SelectedTagGroupValues;
		if (FilteringView->GetSelectedTagGroupValues(SelectedTagGroupValues))
		{
			FilterParams.WithTags(SelectedTagGroupValues);
		}
	}

	return FilterParams;
}
