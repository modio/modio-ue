// Fill out your copyright notice in the Description page of Project Settings.

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
	UFUNCTION(BlueprintCallable)
	bool ToggleDrawerExpanded(int32 SlotIndex, bool bCloseOtherDrawers = true);
	UFUNCTION(BlueprintCallable)
	void SetDrawerExpanded(int32 SlotIndex, bool bExpandedState, bool bCloseOtherDrawers = true);

	UFUNCTION(BlueprintCallable)
	bool IsAnyDrawerExpanded() const;

	UFUNCTION(BlueprintCallable)
	void CollapseAllDrawers();
};
