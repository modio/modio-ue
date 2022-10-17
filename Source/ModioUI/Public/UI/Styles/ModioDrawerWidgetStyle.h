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

#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioDrawerWidgetStyle.generated.h"

/**
* Modio derived struct that defines the properties for a drawer widget with 
* Heading, Content, and Footer with name and brushes for the content to display
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioDrawerWidgetStyle : public FSlateWidgetStyle
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
	* Retrieve a default instance of the modio drawer widget
	**/
	static const FModioDrawerWidgetStyle& GetDefault()
	{
		static FModioDrawerWidgetStyle Default;
		return Default;
	}

	/**
	* Stored property to represent the heading background brush
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FSlateBrush HeadingBackgroundBrush;

	/**
	* Stored property to represent the content background brush
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FSlateBrush ContentBackgroundBrush;

	/**
	* Stored property to represent the footer background brush
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FSlateBrush FooterBackgroundBrush;
};

/**
* Modio element to wrap a FModioDrawerWidgetStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioDrawerWidgetStyle"))
class UModioDrawerWidgetStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property to represent the container style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioDrawerWidgetStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};