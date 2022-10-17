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

#include "Components/OverlaySlot.h"
#include "CoreMinimal.h"
#include "ModioDrawerControllerSlot.generated.h"

/**
* Enum listing the possible alignment locations of a drawer slot
**/
UENUM(BlueprintType)
enum class EModioDrawerControllerSlotEdge : uint8
{
	Top,	/// Horizontal top
	Bottom,	/// Horizontal bottom
	Left,	/// Vertical left
	Right	/// Vertical right
};

/**
* Modio UI element that enables control of content using vertical/horizontal alignment 
**/
UCLASS(HideCategories = "Layout|Overlay Slot")
class MODIOUI_API UModioDrawerControllerSlot : public UOverlaySlot
{
	GENERATED_BODY()
protected:
	TSharedPtr<FActiveTimerHandle> CurrentAnimationTimer;

	virtual void SynchronizeProperties() override;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	EModioDrawerControllerSlotEdge Edge;

	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly,
			  meta = (EditCondition =
						  "Edge==EModioDrawerControllerSlotEdge::Top||Edge==EModioDrawerControllerSlotEdge::Bottom",
					  EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EHorizontalAlignment> DrawerHorizontalAlignment;

	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly,
			  meta = (EditCondition =
						  "Edge==EModioDrawerControllerSlotEdge::Left||Edge==EModioDrawerControllerSlotEdge::Right",
					  EditConditionHides),
			  Category = "Widgets")
	TEnumAsByte<EVerticalAlignment> DrawerVerticalAlignment;

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/// @brief Changing this in the editor allows you to preview the animation transition
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	bool bExpanded = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0.01), Category = "Widgets")
	float AnimationTime = 0.1f;

	UPROPERTY()
	float CurrentAnimationProgress = 1;
	virtual EActiveTimerReturnType TickAnimate(double InCurrentTime, float InDeltaTime, bool bAnimateIn);

	void UpdateWidgetTransform();

	virtual void AnimateIn();
	virtual void AnimateOut();

public:
	/**
	* Change the drawer condition between expanded or not 
	* @param bNewState True for an expanded condition, false to collapse it
	**/
	void SetExpanded(bool bNewState);
	
	/**
	* Retrieve the current drawer condition between expanded or not
	* @return True when expanded, false otherwise
	**/
	bool GetExpandedState();

	/**
	* Change the drawer left edge vertical alignment
	* @param NewVAlign Update to the vertical alignment
	**/
	void SetSlotLeftEdge(EVerticalAlignment NewVAlign);
	
	/**
	* Change the drawer right edge vertical alignment
	* @param NewVAlign Update to the vertical alignment
	**/
	void SetSlotRightEdge(EVerticalAlignment NewVAlign);
	
	/**
	* Change the drawer top edge horizontal alignment
	* @param NewHAlign Update to the horizontal alignment
	**/
	void SetSlotTopEdge(EHorizontalAlignment NewHAlign);
	
	/**
	* Change the drawer bottom edge horizontal alignment
	* @param NewHAlign Update to the horizontal alignment
	**/
	void SetSlotBottomEdge(EHorizontalAlignment NewHAlign);
};
