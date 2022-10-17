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

#include "CoreMinimal.h"
#include "UI/Styles/ModioProceduralBrushParams.h"

#include "ModioNamedGlyphMaterialParams.generated.h"

/**
* Class definition that stores a glyph name and provides a material interface associated
* to that property
**/
UCLASS()
class MODIOUI_API UModioNamedGlyphMaterialParams : public UModioProceduralBrushParams
{
	GENERATED_BODY()
protected:
	UFUNCTION()
	TArray<FString> GetValidGlyphNames();

public:
	/**
	* Retrieve the material for this glyph 
	* @return UMaterialInterface glyph instance used
	**/
	UMaterialInterface* GetMaterialInstance() override;

	/**
	* Stored property of the glyph name
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (GetOptions = "GetValidGlyphNames"), Category = "Widget")
	FName GlyphName;
};
