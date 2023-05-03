// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IModioUIStyleRefSerializer.h"
#include "Misc/EngineVersionComparison.h"
#include "PropertyPathHelpers.h"
#include "Styling/ISlateStyle.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "UI/Styles/ModioComboBoxStyle.h"
#include "UI/Styles/ModioInputMappingGlyph.h"
#include "UI/Styles/ModioProceduralBrushParams.h"
#include "UI/Styles/ModioSlateMaterialBrush.h"
#include "UI/Styles/ModioUIColor.h"
#include "UI/Styles/ModioUIWidgetStyleContainer.h"
#include "ModioMaterialData.generated.h"

/**
 * 
 */

UCLASS()
class MODIOUI_API UModioMaterialData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Meta = (ShowOnlyInnerProperties), Category = "Style")
	TMap<FName, UModioProceduralBrushParams*> NamedBrushMaterialsNew;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Meta = (ShowOnlyInnerProperties), Category = "Style")
	TMap<FName, class UModioRoundedRectangleMaterialParams*> RoundedRectangleParams;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Meta = (ShowOnlyInnerProperties), Category = "Style")
	TMap<FName, class UModioCheckBoxMaterialParams*> CheckBoxMaterialParams;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Style")
	class UMaterialInterface* DefaultRoundedRectangleMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Style")
	class UMaterialInterface* DefaultCheckboxMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Style")
	class UMaterialInterface* PngGlyphMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Style")
	class UMaterialInterface* DefaultGlyphMaterial;

	class UMaterialInterface* GetGlyphMaterial(bool bOverridePlatform = false);
};
