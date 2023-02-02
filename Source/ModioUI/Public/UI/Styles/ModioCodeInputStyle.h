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

#include "ModioCodeInputStyle.generated.h"

// Possibly just reference an existing EditableTextBoxStyle instead
/**
 * Modio derived struct that defines the properties for a input field,
 * with name, material, text spacing and styles to customize them
 **/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioCodeInputStyle : public FTextBlockStyle
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

	// UPROPERTY(BlueprintReadWrite,EditAnywhere, meta=(StyleClass="ModioEditableTextBoxStyle"))
	// FModioUIStyleRef TextBoxStyle;

	/// @brief Brush to use for the box around each character element
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FSlateBrush CharacterBrush;

	/**
	 * Stored property of the color to use when input is focused
	 **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioUIColorRef ColorWhenFocused;

	/**
	 * Stored property of the material reference to use in the fake caret
	 **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FModioUIMaterialRef FakeCaretMaterial;

	/**
	 * Stored property of the margin space between characters
	 **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FMargin CharacterSpacing;

	/**
	 * Stored property of the minimum character size
	 **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FVector2D MinimumCharacterSize {0, 0};

	/**
	 * Retrieve a default instance of the input style
	 **/
	static const FModioCodeInputStyle& GetDefault()
	{
		static FModioCodeInputStyle Default;
		return Default;
	}
};

/**
 * Modio element to wrap a FModioCodeInputStyle using UModioUIWidgetStyleContainer properties
 **/
UCLASS(meta = (DisplayName = "ModioCodeInputStyle"))
class UModioCodeInputStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	/**
	 * Stored property of the code input style to use in this class
	 **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Widget")
	FModioCodeInputStyle Style;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}

	virtual void PostLoad() override
	{
		Super::PostLoad();
	}
};