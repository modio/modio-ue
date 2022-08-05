// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Styles/ModioProceduralBrushParams.h"

#include "ModioNamedGlyphMaterialParams.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUI_API UModioNamedGlyphMaterialParams : public UModioProceduralBrushParams
{
	GENERATED_BODY()
protected:
	UFUNCTION()
	TArray<FString> GetValidGlyphNames();

public:
	UMaterialInterface* GetMaterialInstance() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (GetOptions = "GetValidGlyphNames"), Category="Widget")
	FName GlyphName;
};
