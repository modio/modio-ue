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

#include "ModioUI.h"
#include "ModioUISubsystem.h"
#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"
#include "UI/Default/Search/ModioCommonFilteringView.h"
#include "UI/Default/Search/ModioCommonSearchTabViewStyle.h"
#include "UI/Foundation/Base/ModioCommonWidgetSwitcher.h"
#include "UI/Foundation/Components/Border/ModioCommonBorder.h"
#include "UI/Foundation/Components/Tab/ModioCommonTabListWidgetBase.h"
#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBox.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Settings/ModioCommonUISettings.h"
#include "UI/Settings/Params/ModioCommonSearchParams.h"

const FName SortingTabId = FName(TEXT("Sorting"));
const FName FilteringTabId = FName(TEXT("Filtering"));

void UModioCommonSearchTabView::SetStyle(TSubclassOf<UModioCommonSearchTabViewStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
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

	// Set a default object for the datasource
	DataSource = NewObject<UModioSearchResultsParamsUI>();
	
	if (OverlayBackgroundBorder)
	{
		OverlayBackgroundBorder->OnModioCommonBorderClicked.AddWeakLambda(this, [this]() {
			DeactivateWidget();
		});
	}

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (CloseButton)
		{
			ListenForInputAction(CloseButton, UISettings->SearchParams.CloseInputAction, UISettings->SearchParams.CloseButtonLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				DeactivateWidget();
			}));
		}

		if (SearchButton)
		{
			ListenForInputAction(SearchButton, UISettings->SearchParams.SearchInputAction, UISettings->SearchParams.SearchButtonLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				ShowSearchResults();
			}));
		}

		if (ResetButton)
		{
			ListenForInputAction(ResetButton, UISettings->SearchParams.ResetInputAction, UISettings->SearchParams.ResetButtonLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				Reset();
			}));
		}
	}

	if (SearchTextBox)
	{
		SearchTextBox->OnTextCommitted.AddDynamic(this, &UModioCommonSearchTabView::OnSearchTextCommitted);
	}
}

void UModioCommonSearchTabView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (FilteringView)
	{
		FilteringView->SetDataSource(DataSource);
	}

	if (SearchTextBox)
	{
		UModioSearchResultsParamsUI* SearchResultsParamsUI = Cast<UModioSearchResultsParamsUI>(DataSource);
		if (!SearchResultsParamsUI)
		{
			UE_LOG(ModioUI, Error, TEXT("Unable to set search results params UI on search text box in '%s', data source is either null or not of type UModioSearchResultsParamsUI"), *GetName());
			return;
		}

		SearchTextBox->SetText(FText::FromString(SearchResultsParamsUI->FilterParams.SearchKeywords));
	}
}

void UModioCommonSearchTabView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (CloseButton)
		{
			CloseButton->SetLabel(UISettings->SearchParams.CloseButtonLabel);
		}

		if (SearchButton)
		{
			SearchButton->SetLabel(UISettings->SearchParams.SearchButtonLabel);
		}

		if (ResetButton)
		{
			ResetButton->SetLabel(UISettings->SearchParams.ResetButtonLabel);
		}

		if (SearchTabTitleTextBlock)
		{
			SearchTabTitleTextBlock->SetText(UISettings->SearchParams.SearchTabTitle);
		}
	}

	if (UModioCommonSearchTabViewStyle* StyleCDO = Cast<UModioCommonSearchTabViewStyle>(ModioStyle.GetDefaultObject()))
	{
		if (SearchTabTitleTextBlock)
		{
			SearchTabTitleTextBlock->SetStyle(StyleCDO->SearchTabTitleTextStyle);
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

		if (ResetButton)
		{
			ResetButton->SetStyle(StyleCDO->ResetButtonStyle);
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
		Subsystem->ShowSearchResults(GetFilterParamsWrapper(), false);
	}
}

void UModioCommonSearchTabView::Reset_Implementation()
{
	if (FilteringView)
	{
		FilteringView->ResetFiltering();
	}

	if (SearchTextBox)
	{
		SearchTextBox->SetText(FText::GetEmpty());
	}
}

void UModioCommonSearchTabView::ZeroOut_Implementation()
{
	if (FilteringView)
	{
		FilteringView->ZeroOutFiltering();
	}

	if (SearchTextBox)
	{
		SearchTextBox->SetText(FText::GetEmpty());
	}
}

FModioFilterParams UModioCommonSearchTabView::GetFilterParams_Implementation() const
{
	return GetFilterParamsWrapper().ToFilterParams();
}

FModioModCategoryParams UModioCommonSearchTabView::GetFilterParamsWrapper_Implementation() const
{
	FModioModCategoryParams FilterParams;

	if (FilteringView)
	{
		FilterParams = FilteringView->GetFilterParamsWrapper();
	}

	if (SearchTextBox)
	{
		FilterParams.SearchKeywords = SearchTextBox->GetText().ToString();
	}

	return FilterParams;
}
