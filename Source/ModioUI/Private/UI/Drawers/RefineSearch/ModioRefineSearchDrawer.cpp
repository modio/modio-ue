/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Drawers/RefineSearch/ModioRefineSearchDrawer.h"
#include "UI/CommonComponents/ModioTagSelectorWidgetBase.h"
#include "UI/CommonComponents/ModioSelectableTag.h"
#include "UI/BaseWidgets/ModioGridPanel.h"
#include "UI/BaseWidgets/Slots/ModioDrawerControllerSlot.h"
#include "Types/ModioFilterParams.h"

void UModioRefineSearchDrawer::NativeOnInitialized()
{
	UModioUserWidgetBase::NativeOnInitialized();
	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UModioRefineSearchDrawer::OnApplyClicked);
	}
	if (ClearButton)
	{
		ClearButton->OnClicked.AddDynamic(this, &UModioRefineSearchDrawer::OnClearClicked);
	}

	UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
}

FReply UModioRefineSearchDrawer::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	ConstructNavigationPath();
	ValidateAndSetFocus();
	return FReply::Handled();
}

void UModioRefineSearchDrawer::ConstructNavigationPath() 
{
	NavigationPath.Empty();
	NavigationPath.Add(SearchInput);
	
	for (auto& item : TagSelector->TagSelectorList->GetDisplayedEntryWidgets()) 
	{
		UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(item);
		if (!tagSelector) 
		{
			continue;
		}

		for (auto& widget : tagSelector->CategoryTagList->GetDisplayedEntryWidgets())
		{
			UModioSelectableTag* tag = Cast<UModioSelectableTag>(widget);
			if (!tag) 
			{
				continue;
			}
			NavigationPath.Add(tag->TagSelectedCheckbox);
		}
	}

	NavigationPath.Add(ApplyButton);
	NavigationPath.Add(ClearButton);
}

void UModioRefineSearchDrawer::ValidateAndSetFocus() 
{
	if (NavigationPath.IsValidIndex(CurrentNavIndex))
	{
		if (!NavigationPath[CurrentNavIndex]->TakeWidget()->SupportsKeyboardFocus()) 
		{
			SearchInput->StartInput();
			return;
		}
		NavigationPath[CurrentNavIndex]->SetFocus();

		for (auto& item : TagSelector->TagSelectorList->GetDisplayedEntryWidgets())
		{
			UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(item);
			if (!tagSelector)
			{
				continue;
			}

			if (tagSelector->HasFocusedDescendants())
			{
				TagSelector->CategoryTextBlock->SetText(tagSelector->TagCategoryLabel->GetText());
				break;
			}
		}
	}
}

void UModioRefineSearchDrawer::OnClearClicked()
{
	if (TagSelector)
	{
		TagSelector->ClearSelectedTags();
	}
	if (SearchInput)
	{
		SearchInput->SetText(FText());
	}
}

void UModioRefineSearchDrawer::OnApplyClicked()
{
	Execute_NotifySettingsChanged(
		this,
		FModioFilterParams().NameContains(Execute_GetSearchString(this)).WithTags(Execute_GetSelectedTagValues(this)));

	GEngine->GetEngineSubsystem<UModioUISubsystem>()->CloseSearchDrawer();
}

FReply UModioRefineSearchDrawer::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	ConstructNavigationPath();
	
	if (UModioDrawerControllerSlot* controllerSlot = Cast<UModioDrawerControllerSlot>(Slot)) 
	{
		if (!controllerSlot->GetExpandedState())
		{
			return FReply::Unhandled();
		}
	}

	if (!IsVisible()) 
	{
		return FReply::Unhandled();
	}

	if ((GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Back ||
		 GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::RefineSearch) &&
		InKeyEvent.GetKey().IsGamepadKey() && SearchInput->HasFocusedDescendants())
	{
		ApplyButton->SetFocus();
		return FReply::Unhandled();
	}

	if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Down)
	{
		if (NavigationPath.IsValidIndex(CurrentNavIndex + 1)) {
			CurrentNavIndex += 1;
		}
		ValidateAndSetFocus();
		return FReply::Handled();
	}
	else if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Up) 
	{
		if (NavigationPath.IsValidIndex(CurrentNavIndex - 1))
		{
			CurrentNavIndex -= 1;
		}
		ValidateAndSetFocus();
		return FReply::Handled();
	}
	else if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Left) 
	{
		if (CurrentNavIndex == NavigationPath.Num() - 1) 
		{
			CurrentNavIndex -= 1;
		}
		ValidateAndSetFocus();
		return FReply::Handled();
	}
	else if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Right) 
	{
		if (CurrentNavIndex == NavigationPath.Num() - 2)
		{
			CurrentNavIndex += 1;
		}
		ValidateAndSetFocus();
		return FReply::Handled();
	}

	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

FReply UModioRefineSearchDrawer::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UModioRefineSearchDrawer::NextTagCategory()
{ 
	CategoryNav(true);
}

void UModioRefineSearchDrawer::PrevTagCategory()
{
	CategoryNav(false);
}

void UModioRefineSearchDrawer::CategoryNav(bool bMoveForward)
{
	int currentCategoryIndex = 0;
	bool bCategoryHasSelection = false;
	for (auto& item : TagSelector->TagSelectorList->GetDisplayedEntryWidgets())
	{
		UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(item);
		if (!tagSelector)
		{
			continue;
		}

		if (tagSelector->HasFocusedDescendants())
		{
			bCategoryHasSelection = true;
			break;
		}
		currentCategoryIndex++;
	}

	if (!bCategoryHasSelection)
	{
		currentCategoryIndex = 0;
	}
	currentCategoryIndex += bMoveForward ? 1 : -1;
	if (TagSelector->TagSelectorList->GetDisplayedEntryWidgets().IsValidIndex(bCategoryHasSelection ? currentCategoryIndex : 0))
	{
		UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(
			TagSelector->TagSelectorList->GetDisplayedEntryWidgets()[bCategoryHasSelection ? currentCategoryIndex : 0]);
		if (tagSelector->CategoryTagList->GetDisplayedEntryWidgets().IsValidIndex(0))
		{
			UModioSelectableTag* tag =
				Cast<UModioSelectableTag>(tagSelector->CategoryTagList->GetDisplayedEntryWidgets()[0]);
			if (!tag)
			{
				return;
			}
			NavigationPath.Find(tag->TagSelectedCheckbox, CurrentNavIndex);
			ValidateAndSetFocus();
		}
	}
}

void UModioRefineSearchDrawer::BuildCommandList(TSharedRef<FUICommandList> InCommandList)
{
	InCommandList->MapAction(
		FModioCommonUICommands::Get().NextPage,
		FUIAction(FExecuteAction::CreateUObject(this, &UModioRefineSearchDrawer::NextTagCategory)));
	InCommandList->MapAction(
		FModioCommonUICommands::Get().PreviousPage,
		FUIAction(FExecuteAction::CreateUObject(this, &UModioRefineSearchDrawer::PrevTagCategory)));
	InCommandList->MapAction(
		FModioCommonUICommands::Get().Subscribe,
		FUIAction(FExecuteAction::CreateUObject(this, &UModioRefineSearchDrawer::OnApplyClicked)));
	InCommandList->MapAction(
		FModioCommonUICommands::Get().MoreOptions,
		FUIAction(FExecuteAction::CreateUObject(this, &UModioRefineSearchDrawer::OnClearClicked)));

}

FString UModioRefineSearchDrawer::NativeGetSearchString()
{
	if (SearchInput)
	{
		return SearchInput->GetText().ToString();
	}
	return FString();
}

TArray<FString> UModioRefineSearchDrawer::NativeGetSelectedTagValues()
{
	if (TagSelector)
	{
		return TagSelector->GetSelectedTags();
	}
	return {};
}

void UModioRefineSearchDrawer::OnTagOptionsReceived(FModioErrorCode ec, TOptional<FModioModTagOptions> Options)
{
	if (!ec)
	{
		if (TagSelector)
		{
			TagOptions = NewObject<UModioTagOptionsUI>();
			TagOptions->Underlying = Options.GetValue();
			TagSelector->SetDataSource(TagOptions);
		}
	}
}

void UModioRefineSearchDrawer::NativeRefreshTags()
{
	if (!TagOptions)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			Subsystem->GetModTagOptionsAsync(
				FOnGetModTagOptionsDelegateFast::CreateUObject(this, &UModioRefineSearchDrawer::OnTagOptionsReceived));
		}
	}
	Invalidate(EInvalidateWidgetReason::LayoutAndVolatility | EInvalidateWidgetReason::Paint);
	if (TagSelector)
	{
		TagSelector->InvalidateLayoutAndVolatility();
	}
}

FOnSearchSettingsChanged& UModioRefineSearchDrawer::NativeGetOnSettingsChangedDelegate()
{
	return OnSettingsChanged;
}
