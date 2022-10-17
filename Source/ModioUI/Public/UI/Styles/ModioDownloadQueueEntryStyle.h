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
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioDownloadQueueEntryStyle.generated.h"

/**
* Modio derived struct that defines the properties for a queue entry,
* with name, brush, and styles to customize them
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioDownloadQueueEntryStyle : public FSlateWidgetStyle
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
	* Retrieve a default instance of the download queue entry
	**/
	static const FModioDownloadQueueEntryStyle& GetDefault()
	{
		static FModioDownloadQueueEntryStyle Default;
		return Default;
	}

	/**
	* Stored property to represent the highlight border brush
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FSlateBrush HighlightedBorderBrush;

	/**
	* Stored UI property to represent the unsubscribe button style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioButtonStyle"), Category = "Widget")
	FModioUIStyleRef UnsubscribeButtonStyle = FModioUIStyleRef {"DefaultButtonStyle"};

	/**
	* Stored UI property to represent the text style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category = "Widget")
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};
};

/**
* Modio element to wrap a FModioDownloadQueueEntryStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioDownloadQueueEntryStyle"))
class UModioDownloadQueueEntryStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property to represent the container style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioDownloadQueueEntryStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
