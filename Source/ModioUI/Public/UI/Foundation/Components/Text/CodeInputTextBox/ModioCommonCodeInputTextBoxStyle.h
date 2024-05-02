/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Styling/SlateTypes.h"	
#include "Styling/CoreStyle.h"
#include "ModioCommonCodeInputTextBoxStyle.generated.h"

/**
 * Modio derived struct that defines the properties for a input field,
 * with name, material, text spacing and styles to customize them
 **/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioCommonCodeInputTextBoxInputStyle : public FTextBlockStyle
{
	GENERATED_BODY()
	
	FModioCommonCodeInputTextBoxInputStyle()
	{
		SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 18));
		SetColorAndOpacity(FLinearColor::White);
		SetShadowOffset(FVector2D(1, 1));
		SetShadowColorAndOpacity(FLinearColor::Transparent);
		TransformPolicy = ETextTransformPolicy::None;

		UObject* UnderlineMaterial = FSoftObjectPath(TEXT("/Modio/UI5/Default/Materials/M_UI_Underline.M_UI_Underline")).TryLoad();
		UObject* FakeKaretMaterial = FSoftObjectPath(TEXT("/Modio/UI5/Default/Materials/M_UI_FakeCaret.M_UI_FakeCaret")).TryLoad();
	
		if (UnderlineMaterial)
		{
			CharacterBorderBrushBase.SetResourceObject(UnderlineMaterial);
			CharacterBorderBrushFocused.SetResourceObject(UnderlineMaterial);
		}

		if (FakeKaretMaterial)
		{
			FakeCaretBrush.SetResourceObject(FakeKaretMaterial);
		}

		CharacterBorderBrushBase.TintColor = FLinearColor::White;
		CharacterBorderBrushFocused.TintColor = FLinearColor::Blue;
		FakeCaretBrush.TintColor = FLinearColor::Blue;

		CharacterPadding = FMargin(0, 0, 0, 8);
		CharacterSpacing = FMargin(6, 0, 6, -16);
	}

	/**
	 * Brush to use for the box around each character element, when not focused
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget|Border Brush")
	FSlateBrush CharacterBorderBrushBase;

	/**
	 * Brush to use for the box around each character element, when focused
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget|Border Brush")
	FSlateBrush CharacterBorderBrushFocused;

	/**
	 * Padding to use for the box around each character element
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget|Border Brush")
	FMargin CharacterBorderPadding;

	/**
	 * Fake caret brush (displayed when the input field is focused, at the location of the next character to be entered)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget|FakeCaret")
	FSlateBrush FakeCaretBrush;

	/**
	 * Horizontal alignment of the fake caret
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget|FakeCaret")
	TEnumAsByte<EHorizontalAlignment> FakeCaretHorizontalAlignment = EHorizontalAlignment::HAlign_Fill;

	/**
	 * Vertical alignment of the fake caret
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget|FakeCaret")
	TEnumAsByte<EVerticalAlignment> FakeCaretVerticalAlignment = EVerticalAlignment::VAlign_Fill;

	/**
	 * Character padding around each character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FMargin CharacterPadding;

	/**
	 * Character spacing between each character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FMargin CharacterSpacing;

	/**
	 * Minimum width of each character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	float MinimumCharacterWidth = 0;

	/**
	 * Hint text to display when the input field is empty or in platform-specific hint text locations (such as above the input field)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FText HintText;
};

/**
 * The style of the Code Input Text Box within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonCodeInputTextBoxStyle : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * The style of the input field
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FModioCommonCodeInputTextBoxInputStyle Style;
};
