// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UI/Styles/ModioUIColor.h"
#include "ModioColorPresets.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUI_API UModioColorPresets : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Style")
	TMap<FName, FModioUIColor> ColorPresets;
};
