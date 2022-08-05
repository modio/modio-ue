// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Types/ModioFilterParams.h"

#include "ModioFilterParamsUI.generated.h"

/**
 * Wrapper around a FModioFilterParams because UMG widgets expect UObjects for data sources or list items 
 */
UCLASS(BlueprintType)
class MODIOUI_API UModioFilterParamsUI : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFilterParamsUI")
	FModioFilterParams Underlying;
};
