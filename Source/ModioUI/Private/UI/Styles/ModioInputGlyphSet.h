/*
 *  Copyright (C) 2022 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */


#pragma once

#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "Containers/Map.h"
#include "InputCoreTypes.h"

#include "ModioInputGlyphSet.generated.h"

UCLASS(BlueprintType)
class MODIOUI_API UModioInputGlyphSet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glyphs")
	TMap<FKey, UTexture2D*> Glyphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glyphs")
	TMap<FKey, UTexture2D*> PngGlyphs;

	UMaterialInterface* GetGlyphForKey(const FKey& Key);

	UTexture2D* GetTextureForKey(const FKey& Key);
};