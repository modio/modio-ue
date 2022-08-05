// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/OverlaySlot.h"
#include "CoreMinimal.h"
#include "ModioNotificationControllerSlot.generated.h"

UENUM(BlueprintType)
enum class EModioNotificationControllerSlotEdge : uint8
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
class MODIOUI_API UModioNotificationControllerSlot : public UOverlaySlot
{
	GENERATED_BODY()
protected:
	TSharedPtr<FActiveTimerHandle> CurrentAnimationTimer;
	
	virtual void SynchronizeProperties() override;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widget")
	EModioNotificationControllerSlotEdge Edge;

	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "Edge==EModioNotificationControllerSlotEdge::Top||Edge==EModioNotificationControllerSlotEdge::Bottom", EditConditionHides), Category="Widget")
	TEnumAsByte<EHorizontalAlignment> NotificationHorizontalAlignment;

	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly,
			  meta=(EditCondition = "Edge==EModioNotificationControllerSlotEdge::Left||Edge==EModioNotificationControllerSlotEdge::Right", EditConditionHides), Category="Widget")
	TEnumAsByte<EVerticalAlignment> NotificationVerticalAlignment;

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UPROPERTY(transient)
	float VisibleDuration = 5;

	/// @brief Changing this in the editor allows you to preview the animation transition
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widget")
	bool bExpanded = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(ClampMin=0.01), Category="Widget")
	float AnimationTime = 0.1f;

	UPROPERTY()
	float CurrentAnimationProgress = 0;
	virtual EActiveTimerReturnType TickAnimate(double InCurrentTime, float InDeltaTime, bool bAnimateIn);

	void UpdateWidgetTransform();

	virtual void AnimateIn();

	EActiveTimerReturnType BeginAnimateOut(double, float);
	virtual void AnimateOut();

public:
	bool IsShowing();
	void Show(float InVisibleDuration);
	void SetSlotLeftEdge(EVerticalAlignment NewVAlign);
	void SetSlotRightEdge(EVerticalAlignment NewVAlign);
	void SetSlotTopEdge(EHorizontalAlignment NewHAlign);
	void SetSlotBottomEdge(EHorizontalAlignment NewHAlign);
};
