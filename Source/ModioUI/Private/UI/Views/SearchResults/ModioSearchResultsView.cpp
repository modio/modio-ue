/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/SearchResults/ModioSearchResultsView.h"
#include "Algo/Accumulate.h"
#include "Core/Input/ModioInputKeys.h"
#include "Core/ModioFilterParamsUI.h"
#include "Components/OverlaySlot.h"
#include "UI/CommonComponents/ModioErrorRetryWidget.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioGridPanel.h"
#include "UI/BaseWidgets/ModioPopupComboBox.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioTileView.h"
#include "UI/BaseWidgets/ModioUIAsyncLoader.h"
#include "UI/CommonComponents/ModioMenu.h"
#include "UI/CommonComponents/ModioModTile.h"
#include "Types/ModioModTag.h"
#include "Loc/BeginModioLocNamespace.h"
#include "Widgets/Layout/SGridPanel.h"

void UModioSearchResultsView::NativeOnInitialized()
{
	HasSortActionApplied = false;

	Super::NativeOnInitialized();
	IModioUIModInfoReceiver::Register<UModioSearchResultsView>(EModioUIModInfoEventType::ListAllMods);
	
	if (RefineSearchButton)
	{
		RefineSearchButton->OnClicked.AddDynamic(this, &UModioSearchResultsView::OnRefineSearchButtonClicked);
	}

	if (NoResultsRefineSearchButton)
	{
		NoResultsRefineSearchButton->OnClicked.AddDynamic(this, &UModioSearchResultsView::OnRefineSearchButtonClicked);
	}

	RetryDelegate.Clear();
	RetryDelegate.BindDynamic(this, &UModioSearchResultsView::OnRetryPressed);
	ModioErrorWithRetryWidget->Execute_SetRetryRequestedDelegate(this, RetryDelegate);

	if (ResultLoader)
	{
		TScriptInterface<IModioUIAsyncOperationWidget> InterfaceWrapper =
			TScriptInterface<IModioUIAsyncOperationWidget>(this);
		IModioUIAsyncHandlerWidget::Execute_LinkAsyncOperationWidget(ResultLoader, InterfaceWrapper);
	}

	if (SortBy)
	{
		FModioUIMenuCommandList MenuEntries;

		FModioUIExecuteAction FilterByTrendingDelegate;
		FilterByTrendingDelegate.BindDynamic(this, &UModioSearchResultsView::SortByTrending);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("Trending", "Trending")},
									  FModioUIAction {FilterByTrendingDelegate});

		FModioUIExecuteAction FilterByHighestRated;
		FilterByHighestRated.BindDynamic(this, &UModioSearchResultsView::SortByHighestRated);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("HighestRated", "Highest Rated")},
									  FModioUIAction {FilterByHighestRated});

		FModioUIExecuteAction MostPopular;
		MostPopular.BindDynamic(this, &UModioSearchResultsView::SortByMostPopular);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("MostPopular", "Most Popular")},
									  FModioUIAction {MostPopular});

		FModioUIExecuteAction MostSubscribers;
		MostSubscribers.BindDynamic(this, &UModioSearchResultsView::SortByMostSubscribers);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("MostSubscribers", "Most Subscribers")},
									  FModioUIAction {MostSubscribers});

		FModioUIExecuteAction RecentlyAdded;
		RecentlyAdded.BindDynamic(this, &UModioSearchResultsView::SortByRecentlyAdded);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("RecentlyAdded", "Recently Added")},
									  FModioUIAction {RecentlyAdded});

		FModioUIExecuteAction LastUpdated;
		LastUpdated.BindDynamic(this, &UModioSearchResultsView::SortByLastUpdated);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("LastUpdated", "Last Updated")},
									  FModioUIAction {LastUpdated});

		SortBy->SetOnSelectionChangedWithDelegate(TSlateDelegates<FModioUIAction>::FOnSelectionChanged::CreateUObject(
			this, &UModioSearchResultsView::OnSelectionChanged));

		SortBy->SetComboBoxEntries(MenuEntries);
	}

	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioMenu* MenuInstance = Cast<UModioMenu>(Subsystem->ModBrowserInstance))
		{
			MenuInstance->GetDrawerController()->OnDrawerClosed.AddDynamic(this, &UModioSearchResultsView::OnDrawerClosed);
		}
	}
}

FReply UModioSearchResultsView::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
	if (IsValid(Subsystem) && !(Subsystem->GetLastInputDevice() == EModioUIInputMode::Mouse))
	{
		FSlateApplication::Get().SetUserFocus(0, RefineSearchButton->TakeWidget(), EFocusCause::Navigation);
	}
	else
	{
		SetFocus();
	}
	return FReply::Handled();
}


void UModioSearchResultsView::OnSelectionChanged(FModioUIAction ModioUIAction, ESelectInfo::Type Arg)
{
	CurrentSortAction = ModioUIAction;
	HasSortActionApplied = true;
	NoResultsDialog->SetVisibility(ESlateVisibility::Collapsed);
	CurrentSortAction.ExecuteAction.ExecuteIfBound();
}

void UModioSearchResultsView::SortByTrending()
{
	if (UModioFilterParamsUI* Params = Cast<UModioFilterParamsUI>(DataSource))
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Params->Underlying =
				Params->Underlying.SortBy(EModioSortFieldType::DownloadsToday, EModioSortDirection::Ascending);
			Subsystem->RequestListAllMods(Params->Underlying, Params->Underlying.ToString());
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::InProgress);
		}
	}
}

void UModioSearchResultsView::SortByHighestRated()
{
	if (UModioFilterParamsUI* Params = Cast<UModioFilterParamsUI>(DataSource))
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Params->Underlying =
				Params->Underlying.SortBy(EModioSortFieldType::Rating, EModioSortDirection::Descending);
			Subsystem->RequestListAllMods(Params->Underlying, Params->Underlying.ToString());
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::InProgress);
		}
	}
}

void UModioSearchResultsView::SortByMostPopular()
{
	if (UModioFilterParamsUI* Params = Cast<UModioFilterParamsUI>(DataSource))
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Params->Underlying =
				Params->Underlying.SortBy(EModioSortFieldType::DownloadsTotal, EModioSortDirection::Descending);
			Subsystem->RequestListAllMods(Params->Underlying, Params->Underlying.ToString());
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::InProgress);
		}
	}
}

void UModioSearchResultsView::SortByMostSubscribers()
{
	if (UModioFilterParamsUI* Params = Cast<UModioFilterParamsUI>(DataSource))
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Params->Underlying =
				Params->Underlying.SortBy(EModioSortFieldType::SubscriberCount, EModioSortDirection::Descending);
			Subsystem->RequestListAllMods(Params->Underlying, Params->Underlying.ToString());
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::InProgress);
		}
	}
}

void UModioSearchResultsView::SortByRecentlyAdded()
{
	if (UModioFilterParamsUI* Params = Cast<UModioFilterParamsUI>(DataSource))
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Params->Underlying =
				Params->Underlying.SortBy(EModioSortFieldType::DateMarkedLive, EModioSortDirection::Descending);
			Subsystem->RequestListAllMods(Params->Underlying, Params->Underlying.ToString());
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::InProgress);
		}
	}
}

void UModioSearchResultsView::SortByLastUpdated()
{
	if (UModioFilterParamsUI* Params = Cast<UModioFilterParamsUI>(DataSource))
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Params->Underlying =
				Params->Underlying.SortBy(EModioSortFieldType::DateUpdated, EModioSortDirection::Descending);
			Subsystem->RequestListAllMods(Params->Underlying, Params->Underlying.ToString());
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::InProgress);
		}
	}
}

void UModioSearchResultsView::NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ec,
																  TOptional<FModioModInfoList> List)
{
	UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();

	if (UModioFilterParamsUI* Params = Cast<UModioFilterParamsUI>(DataSource))
	{
		if (Params->Underlying.ToString() == RequestIdentifier)
		{
			if (!ec)
			{
				SearchResults.Empty();
				Algo::Transform(List->GetRawList(), SearchResults, [](const FModioModInfo& ModInfo) {
					UModioModInfoUI* NewObj = NewObject<UModioModInfoUI>();
					NewObj->Underlying = ModInfo;
					return NewObj;
				});
				ResultsTileView->SetListItems(SearchResults);
				ResultsTileView->SetAllNavigationRules(EUINavigationRule::Stop, TEXT("None"));
				ResultsTileView->RequestRefresh();
				if (NoResultsDialog)
				{
					if (SearchResults.Num() > 0)
					{
						NoResultsDialog->SetVisibility(ESlateVisibility::Collapsed);
						RefineSearchButton->SetNavigationRuleBase(EUINavigation::Down, EUINavigationRule::Escape);
						CurrentListIndex = 0;
						for (auto& item : SearchResults)
						{
							if (UModioModTile* widget = Cast<UModioModTile>(ResultsTileView->GetEntryWidgetFromItem(item))) 
							{
								widget->SetAllNavigationRules(EUINavigationRule::Stop, TEXT("None"));
							}
						}

						if (PreviousRequestIdentifier != RequestIdentifier)
						{
							TryNavigateGrid();
						}
					}
					else
					{
						NoResultsDialog->SetVisibility(ESlateVisibility::Visible);
						if (IsValid(Subsystem) && !(Subsystem->GetLastInputDevice() == EModioUIInputMode::Mouse))
						{
							FSlateApplication::Get().SetUserFocus(0, RefineSearchButton->TakeWidget(),
																  EFocusCause::Navigation);
						}
						RefineSearchButton->SetNavigationRuleExplicit(EUINavigation::Down, NoResultsRefineSearchButton);
						if (IsValid(Subsystem) && !(Subsystem->GetLastInputDevice() == EModioUIInputMode::Mouse))
						{
							NoResultsRefineSearchButton->SetKeyboardFocus();
						}
					}
				}

				IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																		   EModioUIAsyncOperationWidgetState::Success);
			}
			else
			{
				IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																		   EModioUIAsyncOperationWidgetState::Error);
				if (IsValid(Subsystem) && !(Subsystem->GetLastInputDevice() == EModioUIInputMode::Mouse))
				{
					ModioErrorWithRetryWidget->RetryButton->SetKeyboardFocus();
					// Show some kind of error?
				}
			}

			PreviousRequestIdentifier = RequestIdentifier;
		}
	}

	if (SearchResultsCount)
	{
		FString Count = FString::Printf(TEXT("(%d)"), SearchResults.Num());
		SearchResultsCount->SetText(FText::FromString(Count));
	}

	IModioUIModInfoReceiver::NativeOnListAllModsRequestCompleted(RequestIdentifier, ec, List);
}

void UModioSearchResultsView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (UModioFilterParamsUI* Params = Cast<UModioFilterParamsUI>(DataSource))
	{
		if (SearchTags)
		{
			TArray<FModioModTag> ConvertedTags;
			TArray<FString> TagArray;

			if (!SearchInputString.IsEmpty())
			{
				FString tagString = FText::Format(SearchInputTagFormatText,  FFormatNamedArguments {{FString("Input"), FFormatArgumentValue(FText::FromString(SearchInputString))}}).ToString();
				TagArray.Add(tagString);
				TagArray.Append(Params->Underlying.Tags);
				Algo::Transform(TagArray, ConvertedTags, [](const FString& InString) { return FModioModTag {InString}; });
				SearchTags->SetTags(ConvertedTags);
			}
			else if (Params->Underlying.Tags.Num() > 0)
			{
				Algo::Transform(Params->Underlying.Tags, ConvertedTags, [](const FString& InString) { return FModioModTag {InString}; });
				SearchTags->SetTags(ConvertedTags);
			}
			else
			{
				TagNames.Empty();
				DefaultTags.Empty();
				TagNames.Add(DefaultTagText);
				Algo::Transform(TagNames, DefaultTags, [](const FString& InString) { return FModioModTag {InString}; });
				SearchTags->SetTags(DefaultTags);
			}
		}

		// If we have a currently assigned sort delegate, then apply it
		if (HasSortActionApplied)
		{
			CurrentSortAction.ExecuteAction.ExecuteIfBound();
		}
		else
		{
			if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
			{
				// Hide NoResultsDialog for new search
				if (NoResultsDialog)
				{
					NoResultsDialog->SetVisibility(ESlateVisibility::Collapsed);
				}
				Subsystem->RequestListAllMods(Params->Underlying, Params->Underlying.ToString());
				IModioUIAsyncOperationWidget::Execute_NotifyOperationState(
					this, EModioUIAsyncOperationWidgetState::InProgress);
			}
		}
	}
}

void UModioSearchResultsView::OnRetryPressed() 
{
	NativeOnSetDataSource();
}

void UModioSearchResultsView::NativeRequestOperationRetry()
{
	if (!DataSource) 
	{
		return;
	}

	if (UModioFilterParamsUI* Params = Cast<UModioFilterParamsUI>(DataSource))
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			// Hide NoResultsDialog for new search
			if (NoResultsDialog)
			{
				NoResultsDialog->SetVisibility(ESlateVisibility::Collapsed);
			}
			Subsystem->RequestListAllMods(Params->Underlying, Params->Underlying.ToString());
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::InProgress);
		}
	}
}

FReply UModioSearchResultsView::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent)
{
	if (ProcessCommandForEvent(InPointerEvent))
	{
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InPointerEvent);
}

FReply UModioSearchResultsView::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (ProcessCommandForEvent(InKeyEvent))
	{
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UModioSearchResultsView::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (!IsVisible())
	{
		return FReply::Unhandled();
	}

	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();

	if (!IsValid(Subsystem))
	{
		return FReply::Handled();
	}

	UModioMenu* Menu = Cast<UModioMenu>(Subsystem->ModBrowserInstance);

	if (IsValid(Menu) && Menu->IsAnyDrawerExpanded())
	{
		return FReply::Handled();
	}

	UModioUI4Subsystem* UI4Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();

	if (IsValid(UI4Subsystem) && UI4Subsystem->IsAnyDialogOpen())
	{
		return FReply::Handled();
	}

	// Had to do grid navigation manually since UE nav is seemingly random at times

	float entryWidth = ResultsTileView->GetEntryWidth();
	float viewWidth = ResultsTileView->GetCachedGeometry().GetLocalSize().X;
	int itemsPerRow = FMath::FloorToInt(viewWidth / entryWidth);

	int currentIndex = ResultsTileView->GetIndexForItem(ResultsTileView->GetSelectedItem());

	if (CurrentRow + CurrentListIndex != currentIndex) 
	{
		if (currentIndex != -1)
		{
			CurrentRow = 0;
			while (CurrentRow + itemsPerRow < currentIndex)
			{
				CurrentRow += itemsPerRow;
			}
			CurrentListIndex = currentIndex-CurrentRow;
		}
		else
		{
			CurrentRow = 0;
			CurrentListIndex = 0;
		}
	}

	if (UModioModTile* widget =
			Cast<UModioModTile>(ResultsTileView->GetEntryWidgetFromItem(ResultsTileView->GetItemAt(CurrentListIndex + CurrentRow))))
	{
		if (!widget->AllowMouseHoverFocus())
		{
			return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
		}
	}
	if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Up)
	{
		if (CurrentRow == 0 && !SortBy->HasFocusedDescendants() && !RefineSearchButton->HasAnyUserFocus())
		{
			RefineSearchButton->SetKeyboardFocus();
			return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
		}

		if (!SortBy->HasFocusedDescendants() && !RefineSearchButton->HasAnyUserFocus())
		{
			SetFocus();
			CurrentRow = (CurrentRow - itemsPerRow >= 0 ? CurrentRow - itemsPerRow : CurrentRow);
			return TryNavigateGrid();
		}
		else
		{
			return TryNavigateGrid();
		}
	}
	else if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Down)
	{
		
		if (RefineSearchButton->HasAnyUserFocus() || (SortBy->HasFocusedDescendants() && !SortBy->IsComboBoxOpen()))
		{
			if (ModioErrorWithRetryWidget->IsVisible())
			{
				ModioErrorWithRetryWidget->SetKeyboardFocus();
				return FReply::Handled();
			}

			return TryNavigateGrid();
		}
		else if (SortBy->HasFocusedDescendants() && SortBy->IsComboBoxOpen())
		{
			return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
		}
		else
		{
			CurrentRow = (CurrentRow + itemsPerRow < ResultsTileView->GetNumItems() ? CurrentRow + itemsPerRow : CurrentRow);
			return TryNavigateGrid();
		}
	}
	else if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Left)
	{
		if (SortBy->HasFocusedDescendants()) 
		{
			SortBy->SetComboBoxOpen(false);
			RefineSearchButton->SetKeyboardFocus();
			return FReply::Handled();
		}
		else if ((!RefineSearchButton->HasAnyUserFocus() && !SortBy->HasFocusedDescendants())) 
		{
			CurrentListIndex = CurrentListIndex - 1 >= 0 ? CurrentListIndex - 1 : itemsPerRow - 1;
			return TryNavigateGrid();
		}
	}
	else if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Right)
	{
		if (SortBy->HasFocusedDescendants())
		{
			SortBy->SetComboBoxOpen(false);
		}

		if ((!RefineSearchButton->HasAnyUserFocus() && !SortBy->HasFocusedDescendants())) 
		{
			CurrentListIndex = CurrentListIndex + 1 < itemsPerRow ? CurrentListIndex + 1 : 0;
			return TryNavigateGrid();
		}
	}

	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

FReply UModioSearchResultsView::TryNavigateGrid() 
{
	if (CurrentListIndex + CurrentRow >= ResultsTileView->GetNumItems())
	{
		CurrentListIndex = 0;
	}

	// Index has to be selected manually since NavigateToIndex does not select the target
	ResultsTileView->NavigateToIndex(CurrentListIndex + CurrentRow);
	ResultsTileView->SetSelectedIndex(CurrentListIndex + CurrentRow);
	return FReply::Handled();
}


void UModioSearchResultsView::OnRefineSearchButtonClicked()
{
	if (UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
	{
		Subsystem->SendCommandToBrowserUI(FModioInputKeys::RefineSearch, 0);
	}
}

FText UModioSearchResultsView::GetPageTitle() const
{
	return PageTitle;
}

void UModioSearchResultsView::OnDrawerClosed() {}

void UModioSearchResultsView::SetSearchInputString(FString input)
{
	SearchInputString = input;
}

#include "Loc/EndModioLocNamespace.h"