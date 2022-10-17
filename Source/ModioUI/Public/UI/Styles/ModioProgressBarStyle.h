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
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioProgressBarStyle.generated.h"

/**
* Modio derived struct that defines the properties for a progress bar,
* with the capacity to customize its background, fill, or marquee images
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioProgressBarStyle : public FProgressBarStyle
{
	GENERATED_BODY()

	/**
	* Default constructor without parameters
	**/
	FModioProgressBarStyle() : FProgressBarStyle()
	{
		SetBackgroundImage(FSlateColorBrush(FLinearColor(0.25, 0.25, 0.25)));
		SetFillImage(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)));
		SetMarqueeImage(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)));
	}

	/**
	* Retrieve a default instance of the progress bar
	* @return A default static progress bar style instance
	**/
	static const FModioProgressBarStyle& GetDefault()
	{
		static FModioProgressBarStyle Default;
		return Default;
	}
};

/**
* Modio element to wrap a FModioProgressBarStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioProgressBarStyle"))
class UModioProgressBarStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
    * Stored property of the progress bar style
    **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioProgressBarStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
