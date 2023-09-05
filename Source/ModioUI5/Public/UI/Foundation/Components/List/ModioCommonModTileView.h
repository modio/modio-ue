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
 * @brief Mod Tile View that displays a list of mod tiles
 */
UCLASS(ClassGroup = "UI", meta = (EntryClass = ModioCommonModEntryBase, Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonModTileView : public UModioCommonTileView, public IModioCommonModListViewInterface
{
	GENERATED_BODY()

public:
	virtual void SetModSelectionByID_Implementation(FModioModID ModID) override;
	virtual void SetFocusOnceListIsPopulated_Implementation(bool bFocus) override;
	virtual void RequestFullClearSelection_Implementation() override;
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting = false) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI")
	bool bFocusOnceListIsPopulatedRequested = false;
};
