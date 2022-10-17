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

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioOverlay.h"

#include "ModioDrawerController.generated.h"

struct EVisibility;
/**
 *
 */
UCLASS()
class MODIOUI_API UModioDrawerController : public UModioOverlay
{
	GENERATED_BODY()

protected:
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual FReply OnMouseUp(const FGeometry&, const FPointerEvent&);
	virtual void UpdateVisibility();

public:
	UFUNCTION(BlueprintCallable, Category = "ModioDrawerController")
	bool ToggleDrawerExpanded(int32 SlotIndex, bool bCloseOtherDrawers = true);
	UFUNCTION(BlueprintCallable, Category = "ModioDrawerController")
	void SetDrawerExpanded(int32 SlotIndex, bool bExpandedState, bool bCloseOtherDrawers = true);

	UFUNCTION(BlueprintCallable, Category = "ModioDrawerController")
	bool IsAnyDrawerExpanded() const;

	UFUNCTION(BlueprintCallable, Category = "ModioDrawerController")
	void CollapseAllDrawers();
};
