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

#include "ModioSubscriptionBadgeStyle.generated.h"

/**
* Modio derived struct that defines the properties for a subscription badge,
* with a label, progress bar, and badge styles
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioSubscriptionBadgeStyle : public FSlateWidgetStyle
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
	* Retrieve a default instance of the subscription badge
	**/
	static const FModioSubscriptionBadgeStyle& GetDefault()
	{
		static FModioSubscriptionBadgeStyle Default;
		return Default;
	}

	/**
	* Stored property of the label style as a text block
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FTextBlockStyle LabelStyle;

	/**
	* Stored property of the label style as UI reference
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category = "Widget")
	FModioUIStyleRef LabelStyleNew;

	/**
	* Stored property of the progress bar style as UI reference
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioProgressBarStyle"), Category = "Widget")
	FModioUIStyleRef ProgressBarStyle;

	/**
	* Stored property of the border style as widget border
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioWidgetBorderStyle BadgeBorderStyle;
};

/**
* Modio element to wrap a FModioSubscriptionBadgeStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioSubscriptionBadgeStyle"))
class UModioSubscriptionBadgeStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property of the subscription badge style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioSubscriptionBadgeStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};