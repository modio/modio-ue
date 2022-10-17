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
#include "UI/Styles/ModioRichTextStyle.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"

#include "ModioTagWidgetStyle.generated.h"

/**
* Modio derived struct that defines the properties for a tag widget,
* with properties for the text, tag and style to customize it
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioTagWidgetStyle : public FSlateWidgetStyle
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
	* Retrieve a default instance of the Modio tag widget style
	**/
	static const FModioTagWidgetStyle& GetDefault()
	{
		static FModioTagWidgetStyle Default;
		return Default;
	}

	/**
	* Stored property of the label style as UI reference
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (StyleClass = "ModioRichTextStyle"))
	FModioUIStyleRef TextStyleNew;

	/**
	* Stored property of the background tag brush 
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FSlateBrush TagBackgroundBrush;

	/**
	* Stored property of the label style as UI reference
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioRichTextStyle TextStyle;

	/**
	* Stored property of the background tag brush 
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioWidgetBorderStyle TagBackgroundStyle;

	/**
	* Stored property of the margin for the content displayed
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FMargin ContentPadding;
};

/**
* Modio element to wrap a FModioTagWidgetStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioTagWidgetStyle"))
class UModioTagWidgetStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property of the table row style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (ShowOnlyInnerProperties))
	FModioTagWidgetStyle Style;

	virtual void PostLoad() override
	{
		Super::PostLoad();
	}

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
