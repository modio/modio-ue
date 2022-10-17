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

#include "Materials/MaterialInstanceDynamic.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"
#include "UObject/SoftObjectPtr.h"

#include "ModioTableRowStyle.generated.h"

/**
* Modio derived struct that defines the properties for a table row,
* with multiple color references for text and background
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioTableRowStyle : public FSlateWidgetStyle
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
	* Retrieve a default instance of the Modio table row style
	**/
	static const FModioTableRowStyle& GetDefault()
	{
		static FModioTableRowStyle Default;
		return Default;
	}

	/**
	* Stored property of the hover background color
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
	FModioUIColorRef BackgroundHoverColor;

	/**
	* Stored property of the selected background color
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
	FModioUIColorRef BackgroundSelectedColor;

	/**
	* Stored property of the background color
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
	FModioUIColorRef BackgroundColor;

	/**
	* Stored property of the hover text color
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
	FModioUIColorRef TextHoverColor;

	/**
	* Stored property of the selected text color
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
	FModioUIColorRef TextSelectedColor;

	/**
	* Stored property of the text color
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
	FModioUIColorRef TextColor;
};

/**
* Modio element to wrap a FModioTableRowStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioTableRowStyle"))
class UModioTableRowStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property of the table row style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioTableRowStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};