/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Components/ListViewBase.h"
#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"

#include "ModioListViewInteger.generated.h"

/**
 * Class wrapper to represent a Modio list view that stores integer values
**/
UCLASS(meta = (EntryInterface = UserObjectListEntry))
// clang-format off
class MODIOUI_API UModioListViewInteger : public UListViewBase, public ITypedUMGListView<TSharedPtr<int64> >
// clang-format on
{
	GENERATED_BODY()

	IMPLEMENT_TYPED_UMG_LIST(TSharedPtr<int64>, MyListView)

protected:
	TArray<TSharedPtr<int64>> ListItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ListView)
	TEnumAsByte<EOrientation> Orientation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ListView)
	ESlateVisibility ScrollbarVisibility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "ListView")
	float EntrySpacing = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ListView")
	bool bOverrideItemSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = bOverrideItemSize), Category = "ListView")
	FVector2D ItemSize;

	void ResetSelectionStateOnDisplayedWidgets();

#if WITH_EDITOR
	virtual void OnRefreshDesignerItems() override;
#endif
	TSharedPtr<SListView<TSharedPtr<int64>>> MyListView;
	virtual TSharedRef<STableViewBase> RebuildListWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UUserWidget& OnGenerateEntryWidgetInternal(TSharedPtr<int64> Item, TSubclassOf<UUserWidget> DesiredEntryClass,
											   const TSharedRef<STableViewBase>& OwnerTable);
	FMargin GetDesiredEntryPadding(TSharedPtr<int64> Item) const override;

public:
	/**
	* Replace the integer list with a new array
	* @param InListItems Array of shared pointers with int64 elements
	**/
	void SetListItems(const TArray<TSharedPtr<int64>>& InListItems);

	/**
	* Replace the item selected in the list. When Index is "out-of-bounds", it defaults to 0
	* @param Index Location in the array for the item to select
	* @param bSelectionState Change the selected state, true for selected, false otherwise
	**/
	void SetItemSelection(int32 Index, bool bSelectionState);

	/**
	* Set as selected an item in the list. When Index is "out-of-bounds", it defaults to 0
	* @param Index Location in the array for the item to select
	**/
	void SelectSingleItem(int32 Index);
};
