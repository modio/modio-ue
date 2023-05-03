// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ModioNamedGlyphsDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUI_API UModioNamedGlyphsDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ShowOnlyInnerProperties), Category = "Style")
	TMap<FName, class UTexture2D*> NamedGlyphs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ShowOnlyInnerProperties), Category = "Style")
	TMap<FName, class UTexture2D*> PNGGlyphs;

	UTexture2D* GetNamedGlyph(FName GlyphName, bool bOverridePlatform = false);
};
