// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ModioWidgetStyleData.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUI_API UModioWidgetStyleData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, meta = (ShowOnlyInnerProperties), Category = "Style")
	TMap<FName, class USlateWidgetStyleContainerBase*> WidgetStyles;

	void LoadStyleBrushes();
};
