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

#include "ModioNotificationStyle.generated.h"

/**
* Modio derived struct that defines the properties for a notification,
* with mutiple properties to customize its success/error state
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioNotificationStyle : public FSlateWidgetStyle
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
	* Retrieve a default instance of the Modio notification
	**/
	static const FModioNotificationStyle& GetDefault()
	{
		static FModioNotificationStyle Default;
		return Default;
	}
	
	// @modio-ue4 this *really* should get factored out into a static helper, huh
	/**
	* Retrieve an array of styles within this widget
	* @return Array with string style names
	**/
	TArray<FString> GetStyleNames() const
	{
		TArray<FString> StyleNames;
		TArray<FName> TmpStyleNames;
		if (const FModioRichTextStyle* RichTextStyle = TextStyleSet.FindStyle<FModioRichTextStyle>())
		{
			RichTextStyle->Styles.GenerateKeyArray(TmpStyleNames);
			for (const FName& Name : TmpStyleNames)
			{
				StyleNames.Add(Name.ToString());
			}
		}

		if (StyleNames.Num() == 0)
		{
			StyleNames.Add("Default");
		}
		return StyleNames;
	}

	/**
	* Stored property of the UI text style reference
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category = "Widget")
	FModioUIStyleRef TextStyleSet;

	/**
	* Stored property for the style of the primary text
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (GetOptions = "GetStyleNames"), Category = "Widget")
	FName PrimaryTextStyleName = FName("default");

	/**
	* Stored property for the style of the secondary text
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (GetOptions = "GetStyleNames"), Category = "Widget")
	FName SecondaryTextStyleName = FName("default");

	/**
	* Stored property to the material reference when a success notification occurs
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioUIMaterialRef ErrorSuccessGlyph;

	/**
	* Stored property to the material reference when an error notification occurs
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioUIMaterialRef ErrorFailureGlyph;

	/**
	* Stored property to the material reference of the notification background
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioUIMaterialRef BackgroundMaterial;

	/**
	* Stored property to the color reference when a success notification occurs
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
	FModioUIColorRef SuccessColor;

	/**
	* Stored property to the color reference when an error notification occurs
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget")
	FModioUIColorRef ErrorColor;

	/**
	* Stored property to the notification margin for the content displayed
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FMargin ContentPadding;
};

/**
* Modio element to wrap a FModioNotificationStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioNotificationStyle"))
class UModioNotificationStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property to the notification style to use in this class
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioNotificationStyle Style;

	virtual void PostLoad() override
	{
		Super::PostLoad();
	}

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
