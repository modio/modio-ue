/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CommonTileView.h"
#include "CoreMinimal.h"
#include "Misc/EngineVersionComparison.h"
#include "SCommonButtonTableRow.h"

#include "ModioCommonTileView.generated.h"

/**
 * This recreates the functionality of the SCommonTileView but adds the ability to scroll to the last selected item when the tile view receives focus
 * @tparam ItemType 
 */
template<typename ItemType>
class SModioCommonTileView : public STileView<ItemType>
{
public:
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override
	{
		if (bScrollToSelectedOnFocus &&
			(InFocusEvent.GetCause() == EFocusCause::Navigation || InFocusEvent.GetCause() == EFocusCause::SetDirectly))
		{
#if UE_VERSION_OLDER_THAN(5, 2, 0)
			if (this->ItemsSource && this->ItemsSource->Num() > 0)
#else
			if (this->HasValidItemsSource() && this->GetItems().Num() > 0)
#endif
			{
#if UE_VERSION_OLDER_THAN(5, 2, 0)
				if (LastSelectedIndex != INDEX_NONE && this->ItemsSource->IsValidIndex(LastSelectedIndex))
#else
				if (LastSelectedIndex != INDEX_NONE && this->GetItems().IsValidIndex(LastSelectedIndex))
#endif
				{
#if UE_VERSION_OLDER_THAN(5, 2, 0)
					ItemType LastSelectedItem = (*this->ItemsSource)[LastSelectedIndex];
#else
					ItemType LastSelectedItem = (this->GetItems())[LastSelectedIndex];
#endif
					this->SetSelection(LastSelectedItem, ESelectInfo::OnNavigation);
					this->RequestNavigateToItem(LastSelectedItem, InFocusEvent.GetUser());
				}
				else if (this->GetNumItemsSelected() == 0)
				{
#if UE_VERSION_OLDER_THAN(5, 2, 0)
					ItemType FirstItem = (*this->ItemsSource)[0];
#else
					ItemType FirstItem = (this->GetItems())[0];
#endif
					this->SetSelection(FirstItem, ESelectInfo::OnNavigation);
					this->RequestNavigateToItem(FirstItem, InFocusEvent.GetUser());
				}
				else
				{
					TArray<ItemType> ItemArray;
					this->GetSelectedItems(ItemArray);

					ItemType FirstSelected = ItemArray[0];
					this->SetSelection(FirstSelected, ESelectInfo::OnNavigation);
					this->RequestNavigateToItem(FirstSelected, InFocusEvent.GetUser());
				}
			}
		}
		bScrollToSelectedOnFocus = true;
		return STileView<ItemType>::OnFocusReceived(MyGeometry, InFocusEvent);
	}

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override
	{
		STileView<ItemType>::OnMouseLeave(MouseEvent);

		if (MouseEvent.IsTouchEvent() && this->HasMouseCapture())
		{
			// Regular list views will clear this flag when the pointer leaves the list. To
			// continue scrolling outside the list, we need this to remain on.
			this->bStartedTouchInteraction = true;
		}
	}

	virtual FReply OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override
	{
		FReply Reply = STileView<ItemType>::OnTouchMoved(MyGeometry, InTouchEvent);

		if (Reply.IsEventHandled() && this->HasMouseCapture())
		{
			bScrollToSelectedOnFocus = false;
			Reply.SetUserFocus(this->AsShared());
		}
		return Reply;
	}

	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override
	{
		return STileView<ItemType>::OnTouchEnded(MyGeometry, InTouchEvent);
	}

protected:
	virtual void Private_SignalSelectionChanged(ESelectInfo::Type SelectInfo) override
	{
		TArray<ItemType> ItemArray;
		this->GetSelectedItems(ItemArray);
		if (ItemArray.IsValidIndex(0))
		{
#if UE_VERSION_OLDER_THAN(5, 2, 0)
			(*this->ItemsSource).Find(ItemArray[0], LastSelectedIndex);
#else
			(this->GetItems()).Find(ItemArray[0], LastSelectedIndex);
#endif
		}
		STileView<ItemType>::Private_SignalSelectionChanged(SelectInfo);
	}

private:
	int32 LastSelectedIndex = INDEX_NONE;
	bool bScrollToSelectedOnFocus = true;
};

class UModioCommonListViewStyle;

/**
 * Tile View that displays a list of tiles
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonTileView : public UCommonTileView
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the tile view within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonListViewStyle> InStyle);

protected:
	/** The style of the tile view within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonListViewStyle> ModioStyle;

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

protected:
	//~ Begin UListViewBase Interface
	virtual TSharedRef<STableViewBase> RebuildListWidget() override;
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	//~ End UListViewBase Interface
};