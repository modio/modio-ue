#include "UI/Views/SearchResults/ModioSearchResultsView.h"
#include "Algo/Accumulate.h"
#include "Core/Input/ModioInputKeys.h"
#include "Core/ModioFilterParamsUI.h"
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
}

void UModioSearchResultsView::OnSelectionChanged(FModioUIAction ModioUIAction, ESelectInfo::Type Arg)
{
	CurrentSortAction = ModioUIAction;
	HasSortActionApplied = true;

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
				ResultsTileView->RequestRefresh();

				if (NoResultsDialog)
				{
					if (SearchResults.Num() > 0)
					{
						NoResultsDialog->SetVisibility(ESlateVisibility::Collapsed);
					}
					else
					{
						NoResultsDialog->SetVisibility(ESlateVisibility::Visible);
					}
				}

				IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																		   EModioUIAsyncOperationWidgetState::Success);
			}
			else
			{
				IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																		   EModioUIAsyncOperationWidgetState::Error);
				// Show some kind of error?
			}
		}
	}
	IModioUIModInfoReceiver::NativeOnListAllModsRequestCompleted(RequestIdentifier, ec, List);
}

void UModioSearchResultsView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (UModioFilterParamsUI* Params = Cast<UModioFilterParamsUI>(DataSource))
	{
		if (SearchQuery)
		{
			SearchQuery->SetText(FText::Format(
				ModQueryFormatText, FFormatNamedArguments {{"Query", FText::FromString(Algo::Accumulate(
																		 Params->Underlying.SearchKeywords, FString(),
																		 [](FString Accumulated, FString Input) {
																			 return Accumulated + " " + Input;
																		 }))}}));
		}
		if (SearchTags)
		{
			TArray<FModioModTag> ConvertedTags;
			Algo::Transform(Params->Underlying.Tags, ConvertedTags,
							[](const FString& InString) { return FModioModTag {InString}; });
			SearchTags->SetTags(ConvertedTags);
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

void UModioSearchResultsView::NativeRequestOperationRetry()
{
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

void UModioSearchResultsView::OnRefineSearchButtonClicked()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->SendCommandToBrowserUI(FModioInputKeys::RefineSearch, 0);
	}
}

TSharedRef<SWidget> UModioSearchResultsView::GetMenuTitleContent()
{
	// clang-format off
	return SNew(SGridPanel) 
		+SGridPanel::Slot(0, 0)
		.HAlign(HAlign_Center)
		[
			SNew(SModioRichTextBlock).StyleReference(&TitleTextStyle).Text_UObject(this, &UModioSearchResultsView::GetPageTitle)
		]
		+SGridPanel::Slot(0,1)
		.HAlign(HAlign_Center)
		[
			SNew(SModioRichTextBlock).StyleReference(&TitleTextStyle).Text_UObject(this, &UModioSearchResultsView::GetModCountText)
		];
	// clang-format on
}

FText UModioSearchResultsView::GetPageTitle() const
{
	return PageTitle;
}

FText UModioSearchResultsView::GetModCountText() const
{
	return FText::FormatNamed(FTextFormat(ModCountFormatText), "ResultCount", FText::AsNumber(SearchResults.Num()));
}

#include "Loc/EndModioLocNamespace.h"