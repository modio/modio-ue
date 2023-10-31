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

#include "ModioUI4Subsystem.h"
#include "UI/CommonComponents/ModioTagSelectorWidgetBase.h"
#include "UI/CommonComponents/ModioSelectableTag.h"
#include "UI/BaseWidgets/ModioGridPanel.h"
#include "UI/BaseWidgets/Slots/ModioDrawerControllerSlot.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Types/ModioFilterParams.h"
#include "UI/BaseWidgets/ModioGridPanel.h"
#include "UI/BaseWidgets/Slots/ModioDrawerControllerSlot.h"
#include "UI/CommonComponents/ModioSelectableTag.h"
#include "UI/CommonComponents/ModioTagSelectorWidgetBase.h"
#include "Widgets/Input/SEditableTextBox.h"

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
	if (SearchInput)
	{
		SearchInput->MaxCharacters = MaxCharacterAmount;
	}
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UModioRefineSearchDrawer::OnCancelClicked);
	}

	UISubsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
}

FReply UModioRefineSearchDrawer::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	ConstructNavigationPath();
	BindCollapseButtons();
	CurrentNavIndex = 0;
	ValidateAndSetFocus();
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UModioRefineSearchDrawer::ConstructNavigationPath()
{
	NavigationPath.Empty();
	NavigationPath.Add(SearchInput);

	// Can't use GetDisplayedEntryWidgets here as it appears to be cursed
	// Suppose we should not use it anywhere since the results are rather random

	TArray<UUserWidget*> widgets;
	for (auto& item : TagSelector->SelectorListScrollBox->GetAllChildren())
	{
		widgets.Add(Cast<UUserWidget>(item));
	}

	for (auto& item : widgets)
	{
		UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(item);
		if (!tagSelector)
		{
			continue;
		}

		NavigationPath.Add(tagSelector->TagCategoryCollapseButton);

		if (tagSelector->CategoryVerticalBox->GetVisibility() != ESlateVisibility::Collapsed)
		{
			for (auto& widget : tagSelector->CategoryVerticalBox->GetAllChildren())
			{
				UModioSelectableTag* tag = Cast<UModioSelectableTag>(widget);
				if (!tag)
				{
					continue;
				}
				NavigationPath.Add(tag->TagSelectedCheckbox);
			}
		}
	}

	NavigationPath.Add(ApplyButton);
	NavigationPath.Add(ClearButton);
	NavigationPath.Add(CancelButton);
}

void UModioRefineSearchDrawer::BindCollapseButtons()
{
	TArray<UUserWidget*> widgets;
	for (auto& item : TagSelector->SelectorListScrollBox->GetAllChildren())
	{
		widgets.Add(Cast<UUserWidget>(item));
	}

	for (auto& item : widgets)
	{
		UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(item);
		if (!tagSelector)
		{
			continue;
		}

		tagSelector->TagCategoryCollapseButton->OnClicked.AddUniqueDynamic(
			this, &UModioRefineSearchDrawer::ConstructNavigationPath);
	}
}

void UModioRefineSearchDrawer::ValidateAndSetFocus()
{
	CurrentNavIndex = FMath::Clamp(CurrentNavIndex, 0, NavigationPath.Num() - 1);

	if (NavigationPath.IsValidIndex(CurrentNavIndex))
	{
		if (!NavigationPath[CurrentNavIndex]->TakeWidget()->SupportsKeyboardFocus())
		{
			SearchInput->StartInput();
			return;
		}

		NavigationPath[CurrentNavIndex]->SetFocus();

		TArray<UUserWidget*> widgets;
		for (auto& item : TagSelector->SelectorListScrollBox->GetAllChildren())
		{
			widgets.Add(Cast<UUserWidget>(item));

			UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(item);
			if (tagSelector)
			{
				tagSelector->ToggleKeybindVisibility(false);
				CurrentActiveTagSelector = nullptr;
			}
		}

		// NavPath widgets are not indexes of TagSelector list. TagSelector contains multiple lists so scrollindex is
		// calculated with these in mind

		int itemIndex = 0;
		for (auto& item : widgets)
		{
			UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(item);
			if (!tagSelector)
			{
				continue;
			}

			if (tagSelector->HasFocusedDescendants())
			{
				TagSelector->CategoryTextBlock->SetText(tagSelector->TagCategoryLabel->GetText());
				// TagSelector->TagSelectorList->SetScrollOffset(itemIndex);
				TagSelector->SelectorListScrollBox->ScrollWidgetIntoView(item, true,
																		 EDescendantScrollDestination::IntoView);
				tagSelector->ToggleKeybindVisibility(true);
				CurrentActiveTagSelector = tagSelector;
				break;
			}
			itemIndex++;
		}
	}
}

void UModioRefineSearchDrawer::OnCollapse()
{
	if (IsValid(CurrentActiveTagSelector))
	{
		CurrentActiveTagSelector->OnCategoryCollapseToggled();
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

void UModioRefineSearchDrawer::OnCancelClicked()
{
	GEngine->GetEngineSubsystem<UModioUI4Subsystem>()->CloseSearchDrawer();
}

void UModioRefineSearchDrawer::OnApplyClicked()
{
	Execute_NotifySettingsChanged(
		this,
		FModioFilterParams().NameContains(Execute_GetSearchString(this)).WithTags(Execute_GetSelectedTagValues(this)));

	GEngine->GetEngineSubsystem<UModioUI4Subsystem>()->CloseSearchDrawer();
}

FReply UModioRefineSearchDrawer::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (UModioDrawerControllerSlot* controllerSlot = Cast<UModioDrawerControllerSlot>(Slot))
	{
		if (!controllerSlot->GetExpandedState())
		{
			return FReply::Unhandled();
		}
	}

	ConstructNavigationPath();

	UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();

	if (IsValid(Subsystem) && Subsystem->IsAnyDialogOpen())
	{
		return FReply::Handled();
	}

	if ((GetCommandKeyForEvent(InKeyEvent).Contains(FModioInputKeys::Back) ||
		 GetCommandKeyForEvent(InKeyEvent).Contains(FModioInputKeys::RefineSearch)) &&
		InKeyEvent.GetKey().IsGamepadKey() && SearchInput->HasFocusedDescendants())
	{
		CurrentNavIndex = NavigationPath.Num() - 3;
		ValidateAndSetFocus();
		return FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Enter && CurrentNavIndex == 0)
	{
		OnApplyClicked();
		return FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Tab && CurrentNavIndex == 0)
	{
		CurrentNavIndex += 1;
		ValidateAndSetFocus();
		return FReply::Handled();
	}

	if (GetCommandKeyForEvent(InKeyEvent).Contains(FModioInputKeys::Down))
	{
		CurrentNavIndex += 1;
		ValidateAndSetFocus();
		return FReply::Handled();
	}
	else if (GetCommandKeyForEvent(InKeyEvent).Contains(FModioInputKeys::Up))
	{
		CurrentNavIndex -= 1;
		ValidateAndSetFocus();
		return FReply::Handled();
	}
	if (CurrentNavIndex >= NavigationPath.Num() - 3)
{
		if (GetCommandKeyForEvent(InKeyEvent).Contains(FModioInputKeys::Left))
		{
			CurrentNavIndex--;
			CurrentNavIndex = FMath::Clamp(CurrentNavIndex, NavigationPath.Num() - 3, NavigationPath.Num() - 1);
			ValidateAndSetFocus();
			return FReply::Handled();
		}
		else if (GetCommandKeyForEvent(InKeyEvent).Contains(FModioInputKeys::Right))
		{
			CurrentNavIndex++;
			CurrentNavIndex = FMath::Clamp(CurrentNavIndex, NavigationPath.Num() - 3, NavigationPath.Num() - 1);
			ValidateAndSetFocus();
			return FReply::Handled();
		}
	}
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

FReply UModioRefineSearchDrawer::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (GetCommandKeyForPointerEvent(InMouseEvent) == FModioInputKeys::Back)
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}

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
	for (auto& item : TagSelector->SelectorListScrollBox->GetAllChildren())
	{
		UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(item);
		if (!IsValid(tagSelector))
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
	if (TagSelector->SelectorListScrollBox->GetAllChildren().IsValidIndex(bCategoryHasSelection ? currentCategoryIndex
																								: 0))
	{
		UModioTagSelectorWidgetBase* tagSelector = Cast<UModioTagSelectorWidgetBase>(
			TagSelector->SelectorListScrollBox->GetAllChildren()[bCategoryHasSelection ? currentCategoryIndex : 0]);

		if (!IsValid(tagSelector))
		{
			return;
		}

		if (tagSelector->CategoryVerticalBox->GetAllChildren().IsValidIndex(0) && !tagSelector->IsCollapsed())
		{
			UModioSelectableTag* tag = Cast<UModioSelectableTag>(tagSelector->CategoryVerticalBox->GetAllChildren()[0]);
			if (!tag)
			{
				return;
			}
			NavigationPath.Find(tag->TagSelectedCheckbox, CurrentNavIndex);
			ValidateAndSetFocus();
		}
		else if (tagSelector->IsCollapsed())
		{
			NavigationPath.Find(tagSelector->TagCategoryCollapseButton, CurrentNavIndex);
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
	InCommandList->MapAction(FModioCommonUICommands::Get().Subscribe,
							 FUIAction(FExecuteAction::CreateUObject(this, &UModioRefineSearchDrawer::OnApplyClicked)));
	InCommandList->MapAction(FModioCommonUICommands::Get().MoreOptions,
							 FUIAction(FExecuteAction::CreateUObject(this, &UModioRefineSearchDrawer::OnClearClicked)));
	InCommandList->MapAction(FModioCommonUICommands::Get().Collapse,
							 FUIAction(FExecuteAction::CreateUObject(this, &UModioRefineSearchDrawer::OnCollapse)));
}

FString UModioRefineSearchDrawer::NativeGetSearchString()
{
	if (SearchInput)
	{
		// return empty string if there are only spaces
		FString testString = SearchInput->GetText().ToString();
		testString.RemoveSpacesInline();
		if (testString.IsEmpty())
		{
			return FString();
		}

		// remove spaces at the beginning
		FString parsedString = SearchInput->GetText().ToString();
		while (parsedString.RemoveFromStart(" "))
			;
		return parsedString;
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