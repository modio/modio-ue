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
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"

#include "ModioButtonStyle.generated.h"

/**
* Modio element that represents the style a button could use, for example
* the text style, border or content padding
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioButtonStyle : public FSlateWidgetStyle
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
	* Retrieve a default instance of the Modio button
	**/
	static const FModioButtonStyle& GetDefault()
	{
		static FModioButtonStyle Default;
		return Default;
	}

	/**
	* Default constructor without parameters
	**/
	FModioButtonStyle() : Super()
	{
		ButtonStyle = FButtonStyle()
						  .SetNormal(FSlateColorBrush(FLinearColor(0.25, 0.25, 0.25)))
						  .SetHovered(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)))
						  .SetPressed(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)));
	}

	/**
	* Stored property for the button style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FButtonStyle ButtonStyle;

	/**
	* Stored property of the UI style reference for the text
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category = "Widgets")
	FModioUIStyleRef RichTextStyle;

	/**
	* Stored property of the UI style reference for the border
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRoundedBorderStyle"), Category = "Widgets")
	FModioUIStyleRef BorderStyle;

	/**
	* Stored property for the margin space
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FMargin ContentPadding;
};

/**
* Modio element to wrap a FModioButtonStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioButtonStyle"))
class UModioButtonStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property button style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widgets")
	FModioButtonStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
