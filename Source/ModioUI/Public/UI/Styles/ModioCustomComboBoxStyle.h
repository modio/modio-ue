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

#include "Styling/CoreStyle.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioCustomComboBoxStyle.generated.h"

/**
* Modio derived struct that defines the properties for a combo box,
* with name, sound, color, text and styles to customize them
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioCustomComboBoxStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	/**
	* Static stored property of the type name
	**/
	static const FName TypeName;
	
	/**
	* Function to retrieve the type name stored property
	* @return FName with the type
	**/
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	/**
	* Retrieve a default instance of the custom combo box
	**/	
	static const FModioCustomComboBoxStyle& GetDefault()
	{
		static FModioCustomComboBoxStyle Default;
		return Default;
	}

	/**
	* Stored UI property that represents the border style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRoundedBorderStyle"), Category = "Widget")
	FModioUIStyleRef BorderStyle;

	/**
	* Stored UI property that represents the row style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioTableRowStyle"), Category = "Widget")
	FModioUIStyleRef RowStyle;

	/**
	* Stored UI property that represents the text style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category = "Widget")
	FModioUIStyleRef TextStyle;

	/**
	* Stored UI property that represents the background color
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioUIColorRef BackgroundColor;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FSlateSound PressedSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FSlateSound HoveredSound;
};

/**
* Modio element to wrap a FModioCustomComboBoxStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioCustomComboBoxStyle"))
class UModioCustomComboBoxStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property of the combo box style to use in this class
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioCustomComboBoxStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
