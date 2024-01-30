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
#include "CommonBorder.h"
#include "ModioCommonBorder.generated.h"

class UModioCommonBorderStyle;

/**
 * Border that can be used in Mod.io UI
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonBorder : public UCommonBorder
{
	GENERATED_BODY()

public:
	UModioCommonBorder();

	/**
	 * Sets the style of the Border within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	void SetStyle(TSubclassOf<UModioCommonBorderStyle> InStyle);

	DECLARE_MULTICAST_DELEGATE(FOnModioCommonBorderClicked);
	/**
	 * Called when the border is clicked
	 */
	FOnModioCommonBorderClicked OnModioCommonBorderClicked;

protected:
	UFUNCTION()
	FEventReply HandleOnMouseButtonDownEvent(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
};
