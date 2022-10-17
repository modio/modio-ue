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

#include "ModioNotificationControllerSlot.generated.h"

/**
* Enumerator for the possible position slots for a notification controller 
**/
UENUM(BlueprintType)
enum class EModioNotificationControllerSlotEdge : uint8
{
	/** Top Notification Controller Slot **/
	Top,

	/** Bottom Notification Controller Slot **/
	Bottom,

	/** Left Notification Controller Slot **/
	Left,

	/** Right Notification Controller Slot **/
	Right
};

/**
* Modio UI element that represents a single instance of a notification
**/
UCLASS(HideCategories = "Layout|Overlay Slot")
class MODIOUI_API UModioNotificationControllerSlot : public UOverlaySlot
{
	GENERATED_BODY()
protected:
	TSharedPtr<FActiveTimerHandle> CurrentAnimationTimer;

	virtual void SynchronizeProperties() override;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
	EModioNotificationControllerSlotEdge Edge;

	/** The alignment of the object horizontally. */
	UPROPERTY(
		EditAnywhere, BlueprintReadOnly,
		meta =
			(EditCondition =
				 "Edge==EModioNotificationControllerSlotEdge::Top||Edge==EModioNotificationControllerSlotEdge::Bottom",
			 EditConditionHides),
		Category = "Widget")
	TEnumAsByte<EHorizontalAlignment> NotificationHorizontalAlignment;

	/** The alignment of the object vertically. */
	UPROPERTY(
		EditAnywhere, BlueprintReadOnly,
		meta =
			(EditCondition =
				 "Edge==EModioNotificationControllerSlotEdge::Left||Edge==EModioNotificationControllerSlotEdge::Right",
			 EditConditionHides),
		Category = "Widget")
	TEnumAsByte<EVerticalAlignment> NotificationVerticalAlignment;

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(transient)
	float VisibleDuration = 5;

	/// @brief Changing this in the editor allows you to preview the animation transition
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
	bool bExpanded = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0.01), Category = "Widget")
	float AnimationTime = 0.1f;

	UPROPERTY()
	float CurrentAnimationProgress = 0;
	virtual EActiveTimerReturnType TickAnimate(double InCurrentTime, float InDeltaTime, bool bAnimateIn);

	void UpdateWidgetTransform();

	virtual void AnimateIn();

	EActiveTimerReturnType BeginAnimateOut(double, float);
	virtual void AnimateOut();

public:
	/**
	* Query the notification controller regarding its "show" status
	* @return True when this element is shown, otherwise false
	**/
	bool IsShowing();

	/**
	* Change the "show" status of the notification controller for a period of time
	* @param InVisibleDuration update the period of time the notification should be visible
	**/
	void Show(float InVisibleDuration);

	/**
	* Change the left edge slot for the vertical alignment
	* @param NewVAlign Updated vertical alignment 
	**/
	void SetSlotLeftEdge(EVerticalAlignment NewVAlign);
	
	/**
	 * Change the right edge slot for the vertical alignment
	 * @param NewVAlign Updated vertical alignment
	 **/
	void SetSlotRightEdge(EVerticalAlignment NewVAlign);
	
	/**
	 * Change the top edge slot for the horizontal alignment
	 * @param NewHAlign Updated horizontal alignment
	 **/
	void SetSlotTopEdge(EHorizontalAlignment NewHAlign);

	/**
	 * Change the bottom edge slot for the horizontal alignment
	 * @param NewHAlign Updated horizontal alignment
	 **/
	void SetSlotBottomEdge(EHorizontalAlignment NewHAlign);
};
