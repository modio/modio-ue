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

#include "Brushes/SlateColorBrush.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateWidgetStyle.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"

#include "ModioComboBoxStyle.generated.h"

/**
* Struct derivate of Widget style to store a combo box style
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioComboBoxStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	/**
	* Default constructor without parameters
	**/
	FModioComboBoxStyle() : Super()
	{
		// Populate the combo box style from defaults
		ComboBoxStyle = FCoreStyle::Get().GetWidgetStyle<FComboBoxStyle>("ComboBox");
		// Set the button style to use flat colours
		ComboBoxStyle.ComboButtonStyle.ButtonStyle.SetNormal(FSlateColorBrush(FLinearColor(0.25, 0.25, 0.25)))
			.SetHovered(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)))
			.SetPressed(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)));
	}

	/**
	* Stored property that represents the appearance of an ComboBox
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FComboBoxStyle ComboBoxStyle;

	/**
	* Stored property that represents the menu border style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FModioWidgetBorderStyle MenuBorderStyle;

	/**
	* Stored property that represents the button border style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FModioWidgetBorderStyle ButtonBorderStyle;

	/**
	* Stored property that uses a table row style for the dropdown item
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FTableRowStyle DropdownItemStyle;
};