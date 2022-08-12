// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Views/CategoryBrowser/ModioFeaturedCategory.h"
#include "Algo/Transform.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Core/ModioModInfoUI.h"
#include "Engine/Engine.h"
#include "Libraries/ModioErrorConditionLibrary.h"
#include "ModioSubsystem.h"
#include "TimerManager.h"
#include "Types/ModioModInfo.h"
#include "UI/CommonComponents/ModioModTile.h"
#include "UI/Interfaces/IModioUIAsyncHandlerWidget.h"
#include "UI/Views/CategoryBrowser/ModioFeaturedCategoryParams.h"
#include "UI/Views/ModDetails/ModioAsyncOpWrapperWidget.h"

void UModioFeaturedCategory::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UModioFeaturedCategoryParams* Settings = Cast<UModioFeaturedCategoryParams>(ListItemObject);
	if (Settings)
	{
		SetDataSource(Settings);
	}
}

void UModioFeaturedCategory::NativeRequestOperationRetry()
{
	NativeOnSetDataSource();
}


void UModioFeaturedCategory::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	if (!DataSource)
	{
		return;
	}

	UModioFeaturedCategoryParams* Settings = Cast<UModioFeaturedCategoryParams>(DataSource);
	if (Settings)
	{
		if (Title)
		{
			Title->SetText(Settings->CategoryName);
		}

		Filter = FModioFilterParams()
					 .WithTags(Settings->Tags)
					 .WithoutTags(Settings->ExcludedTags)
					 .SortBy(Settings->SortField, Settings->Direction)
					 .IndexedResults(0, Settings->Count);
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->RequestListAllMods(Filter, GetName());
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::InProgress);
		}
	}
}

void UModioFeaturedCategory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUIModInfoReceiver::Register<UModioFeaturedCategory>(EModioUIModInfoEventType::ListAllMods);
	if (ItemList)
	{
		// This code and associated sub-handlers should live further up the chain
		ItemList->OnEntryWidgetGenerated().AddUObject(this, &UModioFeaturedCategory::OnItemListEntryGenerated);
		ItemList->OnEntryWidgetReleased().AddUObject(this, &UModioFeaturedCategory::OnItemListEntryReleased);
		ItemList->OnItemSelectionChanged().AddUObject(this,
													  &UModioFeaturedCategory::OnCategorySelectionChangedInternal);
		ItemList->SetConsumeMouseWheel(EConsumeMouseWheel::Never);
		ItemList->OnItemScrolledIntoView().AddUObject(this, &UModioFeaturedCategory::OnCategoryFinishedScrolling);
	}

	if (TileLoader)
	{
		TScriptInterface<IModioUIAsyncOperationWidget> InterfaceWrapper =
			TScriptInterface<IModioUIAsyncOperationWidget>(this);
		IModioUIAsyncHandlerWidget::Execute_LinkAsyncOperationWidget(TileLoader, InterfaceWrapper);
	}

	if (SeeAllButton)
	{
		SeeAllButton->OnClicked.AddDynamic(this, &UModioFeaturedCategory::HandleSeeAllClicked);
	}
	if (NavLeftButton)
	{
		NavLeftButton->OnClicked.AddDynamic(this, &UModioFeaturedCategory::HandleNavLeftClicked);
	}
	if (NavRightButton)
	{
		NavRightButton->OnClicked.AddDynamic(this, &UModioFeaturedCategory::HandleNavRightClicked);
	}
	if (CategoryViewContent)
	{
		CategoryViewContent->SetActualAsyncOperationWidget(TScriptInterface<IModioUIAsyncOperationWidget>(this));
	}
}

void UModioFeaturedCategory::OnItemListEntryGenerated(UUserWidget& GeneratedWidget)
{
	UModioModTile* Entry = Cast<UModioModTile>(&GeneratedWidget);
	if (Entry)
	{
		Entry->OnSubscribeClicked.AddUniqueDynamic(this, &UModioFeaturedCategory::HandleItemListEntrySubscribeClicked);
		Entry->OnDetailsClicked.AddUniqueDynamic(this, &UModioFeaturedCategory::HandleItemListEntryDetailsClicked);
		Entry->OnReportClicked.AddUniqueDynamic(this, &UModioFeaturedCategory::HandleItemListEntryReportClicked);
	}
}

void UModioFeaturedCategory::OnItemListEntryReleased(UUserWidget& GeneratedWidget)
{
	UModioModTile* Entry = Cast<UModioModTile>(&GeneratedWidget);
	if (Entry)
	{
		Entry->OnSubscribeClicked.RemoveDynamic(this, &UModioFeaturedCategory::HandleItemListEntrySubscribeClicked);
		Entry->OnDetailsClicked.RemoveDynamic(this, &UModioFeaturedCategory::HandleItemListEntryDetailsClicked);
		Entry->OnReportClicked.RemoveDynamic(this, &UModioFeaturedCategory::HandleItemListEntryReportClicked);
	}
}

void UModioFeaturedCategory::OnCategoryFinishedScrolling(UObject* CategoryItem, UUserWidget& CategoryItemWidget)
{
	int32 VisibleItemIndex = 0;
	TArray<UUserWidget*> DisplayedWidgets = ItemList->GetDisplayedEntryWidgets();
	DisplayedWidgets.Sort([this](UUserWidget& A, UUserWidget& B) {
		int32 IndexA = INDEX_NONE;
		if (UObject* ItemA = *ItemList->ItemFromEntryWidget(A))
		{
			IndexA = ItemList->GetIndexForItem(ItemA);
		}
		int32 IndexB = INDEX_NONE;
		if (UObject* ItemB = *ItemList->ItemFromEntryWidget(B))
		{
			IndexB = ItemList->GetIndexForItem(ItemB);
		}
		return IndexA < IndexB;
	});
	for (UUserWidget* Widget : DisplayedWidgets)
	{
		if (Widget == &CategoryItemWidget)
		{
			UE_LOG(LogTemp, Display, TEXT("NumEntryWidgets %d, Scrolling VisibleItemIndex %d"),
				   ItemList->GetDisplayedEntryWidgets().Num(), VisibleItemIndex);
			SelectionChangedDelegate.Broadcast(VisibleItemIndex, this);
			return;
		}
		VisibleItemIndex++;
	}
}

void UModioFeaturedCategory::HandleItemListEntrySubscribeClicked(UModioModInfoUI* ModInfo)
{
	// managed by the tile itself
	/*if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->RequestSubscriptionForModID(ModInfo->Underlying.ModId);
	}*/
}

void UModioFeaturedCategory::HandleItemListEntryDetailsClicked(UModioModInfoUI* ModInfo) {}

void UModioFeaturedCategory::HandleItemListEntryReportClicked(UModioModInfoUI* ModInfo) {}

void UModioFeaturedCategory::HandleSeeAllClicked()
{
	UModioFeaturedCategoryParams* Settings = Cast<UModioFeaturedCategoryParams>(DataSource);
	if (Settings)
	{
		Filter = FModioFilterParams()
					 .WithTags(Settings->Tags)
					 .WithoutTags(Settings->ExcludedTags)
					 .SortBy(Settings->SortField, Settings->Direction)
					 .IndexedResults(0, 100);
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->DisplaySearchResults(Filter);
		}
	}
}

void UModioFeaturedCategory::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	bool bPreviousEmitSelectionEvents = bEmitSelectionEvents;
	bEmitSelectionEvents = false;
	if (ItemList)
	{
		ItemList->ClearSelection();
	}
	bEmitSelectionEvents = bPreviousEmitSelectionEvents;
}

void UModioFeaturedCategory::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	if (SelectionIndexDelegate.IsBound() && ItemList)
	{
		int32 StartDisplayedIndex =
			ItemList->GetDisplayedEntryWidgets().Num()
				? ItemList->GetIndexForItem(*ItemList->ItemFromEntryWidget(*(ItemList->GetDisplayedEntryWidgets()[0])))
				: 0;

		int32 PendingSelectionIndex = INDEX_NONE;
		PendingSelectionIndex = SelectionIndexDelegate.Execute();

		if (PendingSelectionIndex != INDEX_NONE)
		{
			ItemList->NavigateToIndex(StartDisplayedIndex + PendingSelectionIndex);
			ItemList->SetSelectedIndex(StartDisplayedIndex + PendingSelectionIndex);
		}
		else
		{
			ItemList->NavigateToIndex(StartDisplayedIndex);
			ItemList->SetSelectedIndex(StartDisplayedIndex);
		}
	}
}

bool UModioFeaturedCategory::NativeSupportsKeyboardFocus() const
{
	return false;
}

FReply UModioFeaturedCategory::NativeOnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Unhandled();
}

void UModioFeaturedCategory::NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ec,
																 TOptional<FModioModInfoList> List)
{
	IModioUIModInfoReceiver::NativeOnListAllModsRequestCompleted(RequestIdentifier, ec, List);
	if (RequestIdentifier == GetName())
	{
		if (!ec)
		{
			TArray<UModioModInfoUI*> WrappedModList;
			Algo::Transform(List.GetValue().GetRawList(), WrappedModList, [](const FModioModInfo& In) {
				UModioModInfoUI* WrappedMod = NewObject<UModioModInfoUI>();
				WrappedMod->Underlying = In;
				return WrappedMod;
			});
			ItemList->SetListItems(WrappedModList);
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::Success);
		}
		else
		{
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::Error);
		}
	}
}

FNavigationReply UModioFeaturedCategory::NativeOnNavigation(const FGeometry& InGeometry,
															const FNavigationEvent& InNavigationEvent)
{
	return Super::NativeOnNavigation(InGeometry, InNavigationEvent);
}

void UModioFeaturedCategory::OnCategorySelectionChangedInternal(UModioTileView::NullableItemType Item)
{
	if (!bEmitSelectionEvents)
	{
		return;
	}
	if (Item)
	{
		int32 VisibleItemIndex = 0;
		TArray<UUserWidget*> DisplayedWidgets = ItemList->GetDisplayedEntryWidgets();
		DisplayedWidgets.Sort([this](UUserWidget& A, UUserWidget& B) {
			int32 IndexA = INDEX_NONE;
			if (UObject* ItemA = *ItemList->ItemFromEntryWidget(A))
			{
				IndexA = ItemList->GetIndexForItem(ItemA);
			}
			int32 IndexB = INDEX_NONE;
			if (UObject* ItemB = *ItemList->ItemFromEntryWidget(B))
			{
				IndexB = ItemList->GetIndexForItem(ItemB);
			}
			return IndexA < IndexB;
		});
		for (UUserWidget* Widget : DisplayedWidgets)
		{
			if (Widget == ItemList->GetEntryWidgetFromItem(Item))
			{
				UE_LOG(LogTemp, Display, TEXT("NumEntryWidgets %d, Selection VisibleItemIndex %d"),
					   ItemList->GetDisplayedEntryWidgets().Num(), VisibleItemIndex);
				SelectionChangedDelegate.Broadcast(VisibleItemIndex, this);
				return;
			}
			VisibleItemIndex++;
		}
	}
	else
	{
		// SelectionChangedDelegate.Broadcast(INDEX_NONE, this);
	}
}

void UModioFeaturedCategory::SetSelectionIndexDelegate(FOnGetSelectionIndex Delegate)
{
	SelectionIndexDelegate = Delegate;
}
