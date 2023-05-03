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

#include "Core/Input/ModioUIInputProcessor.h"
#include "Engine/Texture2D.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/SoftObjectPtr.h"

#include "ModioInputMappingGlyph.generated.h"

/**
* Convenience struct to define the input mapping glyph, in other words
* a 2D texture object pointer to the device type that receives user input
* then translate that to actions on the UI
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioInputMappingGlyph
{
	GENERATED_BODY()

	/// @brief Glyph to display for this input when the user is using a mouse
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glpyphs")
	TSoftObjectPtr<UTexture2D> MouseGlyph;

	/// @brief Glyph to display for this input when the user is using a keyboard
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glpyphs")
	TSoftObjectPtr<UTexture2D> KeyboardGlyph;

	/// @brief Glyph to display for this input when the user is using a controller and the game has informed us that it
	/// is specifically an XBox controller
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glpyphs")
	TSoftObjectPtr<UTexture2D> XBoxControllerGlyph;

	/// @brief Glyph to display for this input when the user is using a controller and the game has informed us that it
	/// is specifically a Playstation controller
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glpyphs")
	TSoftObjectPtr<UTexture2D> PSControllerGlyph;

	/// @brief The glyph to use when we don't have an explicit controller type
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glpyphs")
	TSoftObjectPtr<UTexture2D> FallbackControllerGlyph;

	/**
	* Retrieve the 2D texture corresponding to the input mode provided
	* @param InputMode Enumerator that defines the mode
	* @return Glyph 2D texture that matches the input mode, a nullptr if the mode is not listed
	**/
	UTexture2D* GetInputGlyph(EModioUIInputMode InputMode)
	{
		switch (InputMode)
		{
			case EModioUIInputMode::Mouse:
				return MouseGlyph.IsNull() ? nullptr : MouseGlyph.Get();
				break;
			case EModioUIInputMode::Keyboard:
				return KeyboardGlyph.IsNull() ? nullptr : KeyboardGlyph.Get();
				break;
			case EModioUIInputMode::XBox:
				return XBoxControllerGlyph.IsNull() ? nullptr : XBoxControllerGlyph.Get();
				break;
			case EModioUIInputMode::Playstation:
				return PSControllerGlyph.IsNull() ? nullptr : PSControllerGlyph.Get();
				break;
			case EModioUIInputMode::Controller:
				return FallbackControllerGlyph.IsNull() ? nullptr : FallbackControllerGlyph.Get();
				break;
			default:
				break;
		}
		return nullptr;
	}
};

/**
* Modio class definition that maps a input glyph to with a 2D texture
**/
UCLASS()
class UModioInputMappingGlyphLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	* Retrieve the 2D texture related to the input mode
	* @param MappingGlyph Dictionary that stores references available glyphs
	* @param InputMode A single value within the enumerator input mode
	* @return Glyph 2D texture that matches the input mode, a nullptr if the mode is not listed
	**/
	UFUNCTION(BlueprintCallable, Category = "ModioInputMappingGlyphLibrary")
	static UTexture2D* GetInputGlyph(FModioInputMappingGlyph& MappingGlyph, EModioUIInputMode InputMode)
	{
		return MappingGlyph.GetInputGlyph(InputMode);
	}
};