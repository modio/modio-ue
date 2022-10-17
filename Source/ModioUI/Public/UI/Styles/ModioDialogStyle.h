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

#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioUIBrushRef.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioDialogStyle.generated.h"

/**
* Modio derived struct that defines the properties for a dialog,
* with name, background and styles to customize them
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioDialogStyle : public FSlateWidgetStyle
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
	* Retrieve a default instance of the modio dialog
	**/
	static const FModioDialogStyle& GetDefault()
	{
		static FModioDialogStyle Default;
		return Default;
	}

	/**
	* Stored property that represents the margins for this instance content
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FMargin ContentPadding;

	/**
	* Stored property to represent the background brush
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FSlateBrush BackgroundBrush;

	/**
	* Stored UI property to represent the title text style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category = "Widgets")
	FModioUIStyleRef TitleTextStyle;

	/**
	* Stored UI property to represent the content text style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category = "Widgets")
	FModioUIStyleRef ContentTextStyle;

	/**
	* Stored UI property to represent the button style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioButtonStyle"), Category = "Widgets")
	FModioUIStyleRef ButtonStyle;
};

/**
* Modio element to wrap a FModioDialogStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioDialogStyle"))
class UModioDialogStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property to the dialog style to use
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widgets")
	FModioDialogStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};