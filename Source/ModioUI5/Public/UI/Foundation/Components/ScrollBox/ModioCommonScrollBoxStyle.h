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
 * @brief The style of the Scroll Box within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonScrollBoxStyle : public UObject
{
	GENERATED_BODY()

public:
	/** @brief The style of the scroll box */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Main")
	FScrollBoxStyle Style;

	/** @brief The style of the scroll bar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Main")
	TSubclassOf<UModioCommonScrollBarStyle> ScrollBarStyle;

	/** @brief Visibility of the scroll bar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Scroll")
	ESlateVisibility ScrollBarVisibility;

	/** @brief When mouse wheel events should be consumed. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Scroll")
	EConsumeMouseWheel ConsumeMouseWheel;

	/** @brief Disable to stop scrollbars from activating inertial overscrolling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Scroll")
	bool AllowOverscroll;

	/** @brief Whether to back pad this scroll box, allowing user to scroll backward until child contents are no longer visible */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Scroll")
	bool BackPadScrolling;

	/** @brief Whether to front pad this scroll box, allowing user to scroll forward until child contents are no longer visible */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Scroll")
	bool FrontPadScrolling;
	
	/** @brief True to lerp smoothly when wheel scrolling along the scroll box */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Scroll")
	bool bAnimateWheelScrolling = false;

	/** @brief Sets where to scroll a widget to when using explicit navigation or if ScrollWhenFocusChanges is enabled */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Scroll")
	EDescendantScrollDestination NavigationDestination;

	/**
	 * @brief The amount of padding to ensure exists between the item being navigated to, at the edge of the scrollbox.  Use this if you want to ensure there's a preview of the next item the user could scroll to.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Scroll")
	float NavigationScrollPadding;

	/** @brief Scroll behavior when user focus is given to a child widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Scroll")
	EScrollWhenFocusChanges ScrollWhenFocusChanges;
	
	/** @brief Option to disable right-click-drag scrolling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Scroll")
	bool bAllowRightClickDragScrolling;

	/** @brief The multiplier to apply when wheel scrolling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style|Scroll")
	float WheelScrollMultiplier = 1.f;
};
