/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/Search/ModioCommonSearchResultsView.h"

#include "ModioUI5.h"
#include "Core/ModioFilterParamsUI.h"
#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"
#include "UI/Default/Search/ModioCommonSearchResultsViewStyle.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/List/ModioCommonFilteredModListView.h"
#include "UI/Foundation/Components/Tag/ModioCommonModTagList.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Settings/Params/ModioCommonSearchParams.h"

void UModioCommonSearchResultsView::SetStyle(TSubclassOf<UModioCommonSearchResultsViewStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

void UModioCommonSearchResultsView::ShowSearchView_Implementation()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioCommonModBrowser* ModBrowser = Cast<UModioCommonModBrowser>(Subsystem->ModBrowserInstance))
		{
			ModBrowser->ShowSearchView();
		}
	}
}

void UModioCommonSearchResultsView::NativeOnActivated()
{
	Super::NativeOnActivated();
	ShowSearchView();
}

UWidget* UModioCommonSearchResultsView::NativeGetDesiredFocusTarget() const
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
	if (EditSearchButton)
	{
		return EditSearchButton;
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonSearchResultsView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

#if WITH_EDITOR
	if (IsDesignTime())
	{
		Keywords = PreviewKeywords;
		Tags = PreviewTags;
	}
#endif

	if (const UModioCommonSearchParamsSettings* Settings = GetDefault<UModioCommonSearchParamsSettings>())
	{
		if (KeywordsLabelTextBlock)
		{
			KeywordsLabelTextBlock->SetText(Settings->KeywordsLabel);
		}

		if (CategoriesLabelTextBlock)
		{
			CategoriesLabelTextBlock->SetText(Settings->CategoriesLabel);
		}

		if (EditSearchButton)
		{
			EditSearchButton->SetLabel(Settings->EditSearchButtonLabel);
		}

		if (KeywordsDetailsTextBlock)
		{
			const FText KeywordsDetailsText = Keywords.IsEmpty() ? (Tags.IsEmpty() ? Settings->ShowingAllModsLabel : FText::GetEmpty()) : FText::FromString(Keywords);
			KeywordsDetailsTextBlock->SetText(KeywordsDetailsText);
			KeywordsDetailsTextBlock->SetVisibility(KeywordsDetailsText.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
			if (KeywordsLabelTextBlock)
			{
				KeywordsLabelTextBlock->SetVisibility(Keywords.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
			}
		}
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to synchronize properties for '%s': Settings are invalid"), *GetName());
	}

	if (const UModioCommonSearchResultsViewStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (FilteredModListView)
		{
			FilteredModListView->SetStyle(StyleCDO->FilteredModListViewStyle);
		}

		if (KeywordsLabelTextBlock)
		{
			KeywordsLabelTextBlock->SetStyle(StyleCDO->KeywordsLabelTextStyle);
		}

		if (KeywordsDetailsTextBlock)
		{
			KeywordsDetailsTextBlock->SetStyle(StyleCDO->KeywordsDetailsTextStyle);
		}

		if (CategoriesLabelTextBlock)
		{
			CategoriesLabelTextBlock->SetStyle(StyleCDO->CategoriesLabelTextStyle);
		}

		if (EditSearchButton)
		{
			EditSearchButton->SetStyle(StyleCDO->EditSearchButtonStyle);
		}
	}

	if (CategoriesList)
	{
		CategoriesList->SetTagsString(Tags);
		CategoriesList->SetVisibility(Tags.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		if (CategoriesLabelTextBlock)
		{
			CategoriesLabelTextBlock->SetVisibility(Tags.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to set tags for '%s': CategoriesList is not bound"), *GetName());
	}
}

void UModioCommonSearchResultsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const UModioCommonSearchParamsSettings* Settings = GetDefault<UModioCommonSearchParamsSettings>())
	{
		ListenForInputAction(EditSearchButton, Settings->EditSearchInputAction, Settings->EditSearchButtonLabel, [this]() {
			ShowSearchView();
		});
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to initialize '%s': Settings are invalid"), *GetName());
	}
}

void UModioCommonSearchResultsView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	const UModioCommonSearchParamsSettings* Settings = GetDefault<UModioCommonSearchParamsSettings>();
	if (!Settings)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to set data source for '%s': Settings are invalid"), *GetName());
		return;
	}

	UModioFilterParamsUI* FilterParamsPtr = Cast<UModioFilterParamsUI>(DataSource);
	if (!FilterParamsPtr)
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to set data source for '%s': DataSource is not of type UModioFilterParamsUI"), *GetName());
		return;
	}

	const FModioFilterParams& FilterParams = FilterParamsPtr->Underlying;

	FString NewKeywords;
	for (int32 Index = 0; Index < FilterParamsPtr->Underlying.SearchKeywords.Num(); ++Index)
	{
		NewKeywords += FilterParamsPtr->Underlying.SearchKeywords[Index];
		if (Index < FilterParamsPtr->Underlying.SearchKeywords.Num() - 1)
		{
			NewKeywords += TEXT("; ");
		}
	}

	if (Direction == FilterParams.Direction && Tags == FilterParams.Tags && Keywords == NewKeywords && bWasEverPopulated)
	{
		UE_LOG(ModioUI5, Warning, TEXT("Skipping data source update for '%s': Data source is already up to date"), *GetName());
		return;
	}

	bWasEverPopulated = true;
	Direction = FilterParams.Direction;
	Tags = FilterParams.Tags;
	Keywords = MoveTemp(NewKeywords);

	SynchronizeProperties();

	if (FilteredModListView)
	{
		FilteredModListView->SetDataSource(FilterParamsPtr);
	}
	else
	{
		UE_LOG(ModioUI5, Error, TEXT("Unable to set data source for '%s': FilteredModListView is not bound"), *GetName());
	}
}