// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Styles/ModioProceduralBrushParams.h"
#include "UObject/SoftObjectPtr.h"

#include "ModioSpecifiedMaterialParams.generated.h"

/**
 *
 */
UCLASS(EditInlineNew)
class MODIOUI_API UModioSpecifiedMaterialParams : public UModioProceduralBrushParams
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widget")
	TSoftObjectPtr<UMaterialInterface> SpecifiedMaterial;

	virtual UMaterialInterface* GetMaterialInstance() override;
	virtual void PostLoad() override;
};
