// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioListViewString.h"
#include "UI/BaseWidgets/Slate/SModioListView.h"
#include "UI/BaseWidgets/Slate/SModioObjectTableRow.h"

#if WITH_EDITOR
void UModioListViewString::OnRefreshDesignerItems()
{
	RefreshDesignerItems<TSharedPtr<FString>>(ListItems, []() { return TSharedPtr<FString>(new FString()); });
}
#endif

TSharedRef<STableViewBase> UModioListViewString::RebuildListWidget()
{
	FListViewConstructArgs Args;
	Args.bAllowFocus = false;
	Args.SelectionMode = ESelectionMode::Single;
	Args.bClearSelectionOnClick = false;
	Args.ConsumeMouseWheel = EConsumeMouseWheel::Never;
	Args.bReturnFocusToSelection = false;
	Args.Orientation = Orientation;
	MyListView = StaticCastSharedRef<SListView<TSharedPtr<FString>>>(ITypedUMGListView<TSharedPtr<FString>>::ConstructListView<SModioListView>(this, ListItems, Args));
	if (bOverrideItemSize)
	{
		MyListView->SetItemHeight(EntrySpacing * 0.5f + ItemSize.Y);
		MyListView->SetItemWidth(EntrySpacing * 0.5f + ItemSize.X);
	}
	MyListView->SetScrollbarVisibility(UWidget::ConvertSerializedVisibilityToRuntime(ScrollbarVisibility));
	/*MyListView->SetOnEntryInitialized(
		SListView<TSharedPtr<FString> >::FOnEntryInitialized::CreateUObject(this,
	   &UListView::HandleOnEntryInitializedInternal));*/

	return StaticCastSharedRef<SListView<TSharedPtr<FString>>>(MyListView.ToSharedRef());
}

void UModioListViewString::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (MyListView)
	{
		if (bOverrideItemSize)
		{
			MyListView->SetItemHeight(EntrySpacing * 0.5f + ItemSize.Y);
			MyListView->SetItemWidth(EntrySpacing * 0.5f + ItemSize.X);
		}
		MyListView->SetScrollbarVisibility(UWidget::ConvertSerializedVisibilityToRuntime(ScrollbarVisibility));
	}
}

void UModioListViewString::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyListView.Reset();
}

UUserWidget& UModioListViewString::OnGenerateEntryWidgetInternal(TSharedPtr<FString> Item,
																 TSubclassOf<UUserWidget> DesiredEntryClass,
																 const TSharedRef<STableViewBase>& OwnerTable)
{
	return GenerateTypedEntry<UUserWidget, SModioObjectTableRow<TSharedPtr<FString>>>(DesiredEntryClass, OwnerTable);
}

FMargin UModioListViewString::GetDesiredEntryPadding(TSharedPtr<FString> Item) const
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

void UModioListViewString::SetListItems(const TArray<TSharedPtr<FString>>& InListItems)
{
	ListItems.Empty(InListItems.Num());
	ListItems.Append(InListItems);
	ClearSelection();
	for (UUserWidget* CurrentWidget : GetDisplayedEntryWidgets())
	{
		IUserListEntry::UpdateItemSelection(*CurrentWidget, false);
	}
	RequestRefresh();
}
