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
#include "Brushes/SlateNoResource.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"

#include "ModioCheckBoxStyle.generated.h"

/**
* Modio derived struct that defines the properties for a check box,
* with name, text and styles to customize them
**/

USTRUCT(BlueprintType)
struct MODIOUI_API FModioCheckBoxStyle : public FSlateWidgetStyle
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
	* Retrieve a default instance of the check box
	**/
	static const FModioCheckBoxStyle& GetDefault()
	{
		static FModioCheckBoxStyle Default;
		return Default;
	}

	/**
	* Stored property that represents the appearance of an CheckBox
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FCheckBoxStyle CheckBoxStyle;

	/**
	* Stored property for the margin space
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FMargin ContentPadding;

	/**
	* Stored property of the text style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FTextBlockStyle TextStyle;

	/**
	* Stored property of the UI text style reference
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category = "Widget")
	FModioUIStyleRef TextStyleNew;

	/**
	* Stored property of the widget boder style checkbox
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioWidgetBorderStyle CheckBoxBorderStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (DisplayName = "Pressed Sound"))
	FSlateSound PressedSlateSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (DisplayName = "Hovered Sound"))
	FSlateSound HoveredSlateSound;
};

/**
* Modio element to wrap a FModioCheckBoxStyle using UModioUIWidgetStyleContainer properties
**/ 
UCLASS(meta = (DisplayName = "ModioCheckBoxStyle"))
class UModioCheckBoxStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property of the checkbox style to use in this class
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioCheckBoxStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
