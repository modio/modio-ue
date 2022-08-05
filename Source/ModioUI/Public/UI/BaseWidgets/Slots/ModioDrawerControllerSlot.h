// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/OverlaySlot.h"
#include "CoreMinimal.h"
#include "ModioDrawerControllerSlot.generated.h"

UENUM(BlueprintType)
enum class EModioDrawerControllerSlotEdge : uint8
{
	Top,
	Bottom,
	Left,
	Right
};
/**
 *
 */
UCLASS(HideCategories = "Layout|Overlay Slot")
class MODIOUI_API UModioDrawerControllerSlot : public UOverlaySlot
{
	GENERATED_BODY()
protected:
	TSharedPtr<FActiveTimerHandle> CurrentAnimationTimer;
	
	virtual void SynchronizeProperties() override;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	EModioDrawerControllerSlotEdge Edge;

	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "Edge==EModioDrawerControllerSlotEdge::Top||Edge==EModioDrawerControllerSlotEdge::Bottom", EditConditionHides), Category="Widgets")
	TEnumAsByte<EHorizontalAlignment> DrawerHorizontalAlignment;

	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly,
			  meta=(EditCondition = "Edge==EModioDrawerControllerSlotEdge::Left||Edge==EModioDrawerControllerSlotEdge::Right", EditConditionHides), Category="Widgets")
	TEnumAsByte<EVerticalAlignment> DrawerVerticalAlignment;

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/// @brief Changing this in the editor allows you to preview the animation transition
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	bool bExpanded = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(ClampMin=0.01), Category="Widgets")
	float AnimationTime = 0.1f;

	UPROPERTY()
	float CurrentAnimationProgress = 1;
	virtual EActiveTimerReturnType TickAnimate(double InCurrentTime, float InDeltaTime, bool bAnimateIn);

	void UpdateWidgetTransform();

	virtual void AnimateIn();
	virtual void AnimateOut();

public:
	void SetExpanded(bool bNewState);
	bool GetExpandedState();
	void SetSlotLeftEdge(EVerticalAlignment NewVAlign);
	void SetSlotRightEdge(EVerticalAlignment NewVAlign);
	void SetSlotTopEdge(EHorizontalAlignment NewHAlign);
	void SetSlotBottomEdge(EHorizontalAlignment NewHAlign);
};
