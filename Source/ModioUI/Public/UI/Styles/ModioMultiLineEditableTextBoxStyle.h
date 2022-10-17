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
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"

#include "ModioMultiLineEditableTextBoxStyle.generated.h"

/**
* Modio derived struct that defines the properties for a multiline editable text box,
* with a style to the text state
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioMultiLineEditableTextBoxStyle : public FEditableTextBoxStyle
{
	GENERATED_BODY()
	
	/**
	* Stored property for the label in the button
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
	* Stored property of the text style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioTextWidgetStyle"), Category = "Widget")
	FModioUIStyleRef TextStyle;
	
	/**
	* Retrieve a default instance of the multiline editable text box
	**/
	static const FModioMultiLineEditableTextBoxStyle& GetDefault()
	{
		static FModioMultiLineEditableTextBoxStyle Default;
		return Default;
	}
};

/**
* Modio element to wrap a FModioMultiLineEditableTextBoxStyle using UModioUIWidgetStyleContainer properties
**/
UCLASS(meta = (DisplayName = "ModioMultiLineEditableTextBoxStyle"))
class UModioMultiLineEditableTextBoxStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	
	/**
	* Stored property of the editable text box style
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioMultiLineEditableTextBoxStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};