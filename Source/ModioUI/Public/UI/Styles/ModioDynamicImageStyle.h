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
#include "UI/Styles/ModioWidgetBorderStyle.h"

#include "ModioDynamicImageStyle.generated.h"

/**
* Modio derived struct that defines the properties for a image style,
* with slate brush properties of an image loading or unvailable
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioDynamicImageStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	/**
	* Stored property to represent the image loading brush
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FSlateBrush ImageLoadingBrush;

	/**
	* Stored property to represent the image unavailable brush
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FSlateBrush ImageUnavailableBrush;
};

/**
* Modio element to wrap a FModioDynamicImageStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioDynamicImageStyle"))
class UModioDynamicImageStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property to represent the container style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioDynamicImageStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};