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
#include "Styling/SlateTypes.h"
#include "ModioCommonComboBoxStringStyle.generated.h"

/**
 * The style of the Combo Box String within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonComboBoxStringStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Style")
	FComboBoxStyle WidgetStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Style")
	FTableRowStyle ItemStyle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Content")
	FMargin ContentPadding;

	/** The max height of the combobox list that opens */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Content", AdvancedDisplay)
	float MaxListHeight;

	/**
	 * When false, the down arrow is not generated and it is up to the API consumer
	 * to make their own visual hint that this is a drop down.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Content", AdvancedDisplay)
	bool HasDownArrow;

	/**
	* When false, directional keys will change the selection. When true, ComboBox 
	* must be activated and will only capture arrow input while activated.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Content", AdvancedDisplay)
	bool EnableGamepadNavigationMode;

	/**
	 * The default font to use in the combobox, only applies if you're not implementing OnGenerateWidgetEvent
	 * to factory each new entry.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mod.io Common UI|Style")
	FSlateFontInfo Font;

	/** The foreground color to pass through the hierarchy. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mod.io Common UI|Style", meta=(DesignerRebuild))
	FSlateColor ForegroundColor;
};
