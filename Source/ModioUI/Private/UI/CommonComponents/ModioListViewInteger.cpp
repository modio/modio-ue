// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioListViewInteger.h"

void UModioListViewInteger::ResetSelectionStateOnDisplayedWidgets()
{
	for (UUserWidget* CurrentWidget : GetDisplayedEntryWidgets())
	{
		IUserListEntry::UpdateItemSelection(*CurrentWidget, false);
	}
}

#if WITH_EDITOR

void UModioListViewInteger::OnRefreshDesignerItems()
{
	RefreshDesignerItems<TSharedPtr<int64>>(ListItems, []() { return TSharedPtr<int64>(new int64(0)); });
}
#endif

TSharedRef<STableViewBase> UModioListViewInteger::RebuildListWidget()
{
	FListViewConstructArgs Args;
	Args.bAllowFocus = false;
	Args.SelectionMode = ESelectionMode::Single;
	Args.bClearSelectionOnClick = false;
	Args.ConsumeMouseWheel = EConsumeMouseWheel::Never;
	Args.bReturnFocusToSelection = false;
	Args.Orientation = Orientation;
	MyListView = ITypedUMGListView<TSharedPtr<int64>>::ConstructListView<SListView>(this, ListItems, Args);
	if (bOverrideItemSize)
	{
		MyListView->SetItemHeight(EntrySpacing * 0.5f + ItemSize.Y);
		MyListView->SetItemWidth(EntrySpacing * 0.5f + ItemSize.X);
	}
	MyListView->SetScrollbarVisibility(UWidget::ConvertSerializedVisibilityToRuntime(ScrollbarVisibility));
	/*MyListView->SetOnEntryInitialized(
		SListView<TSharedPtr<int64> >::FOnEntryInitialized::CreateUObject(this,
	   &UListView::HandleOnEntryInitializedInternal));*/

	return StaticCastSharedRef<SListView<TSharedPtr<int64>>>(MyListView.ToSharedRef());
}

void UModioListViewInteger::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (bOverrideItemSize)
	{
		MyListView->SetItemHeight(EntrySpacing * 0.5f + ItemSize.Y);
		MyListView->SetItemWidth(EntrySpacing * 0.5f + ItemSize.X);
	}
	MyListView->SetScrollbarVisibility(UWidget::ConvertSerializedVisibilityToRuntime(ScrollbarVisibility));
}

void UModioListViewInteger::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyListView.Reset();
}

UUserWidget& UModioListViewInteger::OnGenerateEntryWidgetInternal(TSharedPtr<int64> Item,
																  TSubclassOf<UUserWidget> DesiredEntryClass,
																  const TSharedRef<STableViewBase>& OwnerTable)
{
	return GenerateTypedEntry<UUserWidget, SObjectTableRow<TSharedPtr<int64>>>(DesiredEntryClass, OwnerTable);
}

FMargin UModioListViewInteger::GetDesiredEntryPadding(TSharedPtr<int64> Item) const
{
	if (ListItems.Num() > 0 && ListItems[0] != Item)
	{
		if (Orientation == EOrientation::Orient_Horizontal)
		{
			// For all entries after the first one, add the spacing as left padding
			return FMargin(EntrySpacing, 0.f, 0.0f, 0.f);
		}
		else
		{
			// For all entries after the first one, add the spacing as top padding
			return FMargin(0.f, EntrySpacing, 0.f, 0.f);
		}
	}

	return FMargin(0.f);
}

void UModioListViewInteger::SetListItems(const TArray<TSharedPtr<int64>>& InListItems)
{
	ListItems.Empty(InListItems.Num());
	ListItems.Append(InListItems);
	ClearSelection();
	ResetSelectionStateOnDisplayedWidgets();
	RequestRefresh();
}

void UModioListViewInteger::SetItemSelection(int32 Index, bool bSelectionState)
{
	if (Index < 0)
	{
		Index = ListItems.Num() - 1;
	}
	if (Index >= ListItems.Num())
	{
		Index = 0;
	}
	MyListView->SetItemSelection(ListItems[Index], bSelectionState);
}

void UModioListViewInteger::SelectSingleItem(int32 Index)
{
	if (Index < 0)
	{
		Index = ListItems.Num() - 1;
	}
	if (Index >= ListItems.Num())
	{
		Index = 0;
	}
	MyListView->ClearSelection();
	ResetSelectionStateOnDisplayedWidgets();
	MyListView->SetSelection(ListItems[Index]);
}
