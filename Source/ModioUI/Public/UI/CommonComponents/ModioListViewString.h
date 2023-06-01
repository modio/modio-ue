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

#include "ModioListViewString.generated.h"

/**
* Modio UI element to wrap a list of viewable strings provided as an
* array of shared string pointers
**/
UCLASS(meta = (EntryInterface = ModioUIUserStringListEntry))
// clang-format off
class MODIOUI_API UModioListViewString : public UListViewBase, public ITypedUMGListView<TSharedPtr<FString> >
// clang-format on
{
	GENERATED_BODY()

	IMPLEMENT_TYPED_UMG_LIST(TSharedPtr<FString>, MyListView)

protected:
	TArray<TSharedPtr<FString>> ListItems;

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

#if WITH_EDITOR
	virtual void OnRefreshDesignerItems() override;
#endif
	TSharedPtr<SListView<TSharedPtr<FString>>> MyListView;
	virtual TSharedRef<STableViewBase> RebuildListWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UUserWidget& OnGenerateEntryWidgetInternal(TSharedPtr<FString> Item, TSubclassOf<UUserWidget> DesiredEntryClass,
											   const TSharedRef<STableViewBase>& OwnerTable);
	FMargin GetDesiredEntryPadding(TSharedPtr<FString> Item) const override;

public:
	/**
	* This function updates the list of items
	* @param InListItems An array of TSharedPtr with the strings to modify the item list
	**/
	void SetListItems(const TArray<TSharedPtr<FString>>& InListItems);
	TArray<TSharedPtr<FString>> GetListItems();
};
