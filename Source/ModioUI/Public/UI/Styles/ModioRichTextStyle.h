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
#include "Styling/SlateStyle.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioTextBlockStyleOverride.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioRichTextStyle.generated.h"

/**
* Modio derived struct that defines the properties for a rich text style,
* with the capacity to customize the text displayed with multiple styles
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioRichTextStyle : public FSlateWidgetStyle
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
	* Retrieve a default instance of the rich text style
	**/
	static const FModioRichTextStyle& GetDefault()
	{
		static FModioRichTextStyle Default;
		return Default;
	}

	/**
	* Retrieve an array of supported styles within this widget
	* @return Array with string style names
	**/
	static const TArray<FString>& GetSupportedTags()
	{
		static TArray<FString> TagList = {"img", "msdf", "override", "bold"};
		return TagList;
	}

	/**
	* Default ModioRichTextStyle constructor with styles for the rich text to display
	**/
	FModioRichTextStyle() : FSlateWidgetStyle()
	{
		Styles.Add("Default", FTextBlockStyle());
		Styles.Add("Paragraph", FTextBlockStyle());
		Styles.Add("H3", FTextBlockStyle());
		Styles.Add("H2", FTextBlockStyle());
		Styles.Add("H1", FTextBlockStyle());
		Styles.Add("Default_Semibold", FTextBlockStyle());
	}

	/**
	* Retrieve the set of styles within this widget
	* @return Shared pointer of  with string style names
	**/
	TSharedPtr<class FSlateStyleSet> GetStyleSet() const
	{
		TSharedPtr<class FSlateStyleSet> StyleInstance = MakeShared<FSlateStyleSet>("ModioRichTextStyleSet");

		for (const auto& Style : Styles)
		{
			if (!StyleInstance->HasWidgetStyle<FTextBlockStyle>(Style.Key))
			{
				StyleInstance->Set(Style.Key, Style.Value);
			}
			else
			{
				const_cast<FTextBlockStyle&>(StyleInstance->GetWidgetStyle<FTextBlockStyle>(Style.Key)) = Style.Value;
			}
		}

		for (const auto& StyleOverride : InlineStyleOverrides)
		{
			if (!StyleInstance->HasWidgetStyle<FModioTextBlockStyleOverride>(StyleOverride.Key))
			{
				StyleInstance->Set(StyleOverride.Key, StyleOverride.Value);
			}
			else
			{
				const_cast<FModioTextBlockStyleOverride&>(StyleInstance->GetWidgetStyle<FModioTextBlockStyleOverride>(
					StyleOverride.Key)) = StyleOverride.Value;
			}
		}
		// This also needs to have the FModioTextBlockStyleOverrides added
		return StyleInstance;
	}

	/**
	* Stored property dictionary between name and text block style 
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	TMap<FName, FTextBlockStyle> Styles;

	/// @brief Inline Style overrides (Should this be moved someplace else perhaps?)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	TMap<FName, FModioTextBlockStyleOverride> InlineStyleOverrides;
};

/**
* Modio element to wrap a FModioRichTextStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioRichTextStyle"))
class UModioRichTextStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	* Stored property to the rich text style to use in this class
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widgets")
	FModioRichTextStyle Style;

	virtual void PostLoad() override
	{
		Super::PostLoad();
	}

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
