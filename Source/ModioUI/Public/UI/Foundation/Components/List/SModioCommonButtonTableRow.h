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

#include "CoreMinimal.h"
#include "ModioUI.h"
#include "SCommonButtonTableRow.h"

/**
 * This is a wrapper around SCommonButtonTableRow that checks if the item is valid before calling any functions to avoid crashes
 * @tparam ItemType The type of item that the row represents
 */
template <typename ItemType>
class SModioCommonButtonTableRow : public SCommonButtonTableRow<ItemType>
{
public:
	SLATE_BEGIN_ARGS(SModioCommonButtonTableRow<ItemType>)
		:_bAllowDragging(true)
	{}
	SLATE_ARGUMENT(bool, bAllowDragging)
	SLATE_EVENT(FOnRowHovered, OnHovered)
	SLATE_EVENT(FOnRowHovered, OnUnhovered)
	SLATE_DEFAULT_SLOT(FArguments, Content)
SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, UUserWidget& InChildWidget, UListViewBase* InOwnerListView)
	{
		SCommonButtonTableRow<ItemType>::Construct(
			typename SCommonButtonTableRow<ItemType>::FArguments()
			.bAllowDragging(InArgs._bAllowDragging)
			.OnHovered(InArgs._OnHovered)
			.OnUnhovered(InArgs._OnUnhovered)
			[
				InArgs._Content.Widget
			], 
			InOwnerTableView, InChildWidget, InOwnerListView);

		if (!IsItemValid())
		{
			UE_LOG(ModioUI, Error, TEXT("SModioCommonButtonTableRow::Construct called with invalid item"));
			this->OwnerTablePtr.Reset();
		}
	}

	bool IsItemValid() const
	{
		TSharedPtr<ITypedTableView<ItemType>> OwnerTable = this->OwnerTablePtr.Pin();
		if (OwnerTable.IsValid())
		{
			if (OwnerTable->Private_ItemFromWidget(this))
			{
				return true;
			}
		}
		return false;
	}

	//~ Begin ITableRow Interface
	virtual bool IsItemExpanded() const override
	{
		if (IsItemValid())
		{
			return SCommonButtonTableRow<ItemType>::IsItemExpanded();
		}
		return false;
	}

	virtual void ToggleExpansion() override
	{
		if (IsItemValid())
		{
			return SCommonButtonTableRow<ItemType>::ToggleExpansion();
		}
	}

	virtual bool IsItemSelected() const override
	{
		if (IsItemValid())
		{
			return SCommonButtonTableRow<ItemType>::IsItemSelected();
		}
		return false;
	}

	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override
	{
		if (IsItemValid())
		{
			return SCommonButtonTableRow<ItemType>::OnMouseButtonDoubleClick(InMyGeometry, InMouseEvent);
		}
		return FReply::Unhandled();
	}

	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override
	{
		if (IsItemValid())
		{
			return SCommonButtonTableRow<ItemType>::OnTouchEnded(MyGeometry, InTouchEvent);
		}
		return FReply::Unhandled();
	}

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (IsItemValid())
		{
			return SCommonButtonTableRow<ItemType>::OnMouseButtonDown(MyGeometry, MouseEvent);
		}
		return FReply::Unhandled();
	}

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (IsItemValid())
		{
			return SCommonButtonTableRow<ItemType>::OnMouseButtonUp(MyGeometry, MouseEvent);
		}
		return FReply::Unhandled();
	}

	virtual void InitializeObjectRow() override
	{
		if (IsItemValid())
		{
			return SCommonButtonTableRow<ItemType>::InitializeObjectRow();
		}
	}
	//~ End ITableRow Interface
};
