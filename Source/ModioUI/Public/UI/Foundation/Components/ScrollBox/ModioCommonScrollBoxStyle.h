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
#include "UObject/Object.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Templates/SubclassOf.h"
#include "Styling/SlateTypes.h"
#include "Components/SlateWrapperTypes.h"
#include "ModioCommonScrollBoxStyle.generated.h"

class UModioCommonScrollBarStyle;

/**
 * The style of the Scroll Box within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonScrollBoxStyle : public UObject
{
	GENERATED_BODY()

public:
	/** The style of the scroll box */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Main")
	FScrollBoxStyle Style;

	/** The style of the scroll bar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Main")
	TSubclassOf<UModioCommonScrollBarStyle> ScrollBarStyle;

	/** Visibility of the scroll bar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll")
	ESlateVisibility ScrollBarVisibility;

	/** When mouse wheel events should be consumed. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll")
	EConsumeMouseWheel ConsumeMouseWheel;

	/** Disable to stop scrollbars from activating inertial overscrolling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll")
	bool AllowOverscroll;

	/** Whether to back pad this scroll box, allowing user to scroll backward until child contents are no longer visible */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll")
	bool BackPadScrolling;

	/** Whether to front pad this scroll box, allowing user to scroll forward until child contents are no longer visible */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll")
	bool FrontPadScrolling;
	
	/** True to lerp smoothly when wheel scrolling along the scroll box */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll")
	bool bAnimateWheelScrolling = false;

	/** Sets where to scroll a widget to when using explicit navigation or if ScrollWhenFocusChanges is enabled */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll")
	EDescendantScrollDestination NavigationDestination;

	/**
	 * The amount of padding to ensure exists between the item being navigated to, at the edge of the scrollbox.  Use this if you want to ensure there's a preview of the next item the user could scroll to.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll")
	float NavigationScrollPadding;

	/** Scroll behavior when user focus is given to a child widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll")
	EScrollWhenFocusChanges ScrollWhenFocusChanges;
	
	/** Option to disable right-click-drag scrolling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll")
	bool bAllowRightClickDragScrolling;

	/** The multiplier to apply when wheel scrolling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll")
	float WheelScrollMultiplier = 1.f;

	/** Whether to scroll using the controller/keyboard navigation input. If true, the scroll box will scroll content when there's no visible/focusable widget in the navigated direction but there's room to scroll */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Style|Scroll|Navigation")
	bool bScrollByNavigationInput = false;

	/** The amount to scroll by the controller/keyboard input when there's no visible/focusable widget in the navigated direction but there's room to scroll. Only relevant if bScrollByNavigationInput is true */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bScrollByNavigationInput"), Category = "Mod.io Common UI|Style|Scroll|Navigation")
	int32 NavigationScrollOffsetStep = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.0, UIMax = 1.0), Category = "Mod.io Common UI|Style|Scroll|Navigation")
	float NavigationScrollOffsetSpeed = 0.1f;
};
