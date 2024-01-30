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
#include "ModioCommonModListViewInterface.h"
#include "ModioCommonTileView.h"

#include "ModioCommonModTileView.generated.h"


/**
 * Mod Tile View that displays a list of mod tiles
 */
UCLASS(ClassGroup = "UI", meta = (EntryClass = ModioCommonModEntryBase, Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModTileView : public UModioCommonTileView, public IModioCommonModListViewInterface
{
	GENERATED_BODY()

public:
	//~ Begin IModioCommonModListViewInterface Interface
	virtual UListView* GetListView() const override { return const_cast<UModioCommonModTileView*>(this); }
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting) override;
	virtual void SetFocusOnceListIsPopulated_Implementation(bool bFocus) override;
	virtual void OnSelectionChangedInternal(NullableItemType FirstSelectedItem) override;
	//~ End IModioCommonModListViewInterface Interface

protected:
	/** Whether to focus the previously selected item when the list is populated with new items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI")
	bool bFocusOnceListIsPopulatedRequested;
};
