#pragma once

#include "Core/Input/ModioUIInputProcessor.h"
#include "Engine/Texture2D.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/SoftObjectPtr.h"

#include "ModioInputMappingGlyph.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioInputMappingGlyph
{
	GENERATED_BODY()

	/// @brief Glyph to display for this input when the user is using a mouse
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Glpyphs")
	TSoftObjectPtr<UTexture2D> MouseGlyph;

	/// @brief Glyph to display for this input when the user is using a keyboard
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Glpyphs")
	TSoftObjectPtr<UTexture2D> KeyboardGlyph;

	/// @brief Glyph to display for this input when the user is using a controller and the game has informed us that it
	/// is specifically an XBox controller
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Glpyphs")
	TSoftObjectPtr<UTexture2D> XBoxControllerGlyph;

	/// @brief Glyph to display for this input when the user is using a controller and the game has informed us that it
	/// is specifically a Playstation controller
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Glpyphs")
	TSoftObjectPtr<UTexture2D> PSControllerGlyph;

	/// @brief The glyph to use when we don't have an explicit controller type
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Glpyphs")
	TSoftObjectPtr<UTexture2D> FallbackControllerGlyph;

	UTexture2D* GetInputGlyph(EModioUIInputMode InputMode)
	{
		switch (InputMode)
		{
			case EModioUIInputMode::Mouse:
				return MouseGlyph.IsNull() ? nullptr : MouseGlyph.LoadSynchronous();
				break;
			case EModioUIInputMode::Keyboard:
				return KeyboardGlyph.IsNull() ? nullptr : KeyboardGlyph.LoadSynchronous();
				break;
			case EModioUIInputMode::XBox:
				return XBoxControllerGlyph.IsNull() ? nullptr : XBoxControllerGlyph.LoadSynchronous();
				break;
			case EModioUIInputMode::Playstation:
				return PSControllerGlyph.IsNull() ? nullptr : PSControllerGlyph.LoadSynchronous();
				break;
			case EModioUIInputMode::Controller:
				return FallbackControllerGlyph.IsNull() ? nullptr : FallbackControllerGlyph.LoadSynchronous();
				break;
			default:
				break;
		}
		return nullptr;
	}
};

UCLASS()
class UModioInputMappingGlyphLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="ModioInputMappingGlyphLibrary")
	static UTexture2D* GetInputGlyph(FModioInputMappingGlyph& MappingGlyph, EModioUIInputMode InputMode)
	{
		return MappingGlyph.GetInputGlyph(InputMode);
	}
};