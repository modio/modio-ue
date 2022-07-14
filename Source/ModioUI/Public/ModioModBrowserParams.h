// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UI/Views/CategoryBrowser/ModioFeaturedCategoryParams.h"
#include "UObject/NoExportTypes.h"

#include "ModioModBrowserParams.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class MODIOUI_API UModioModBrowserParams : public UDataAsset
{
	GENERATED_BODY()
public:
	UModioModBrowserParams();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Featured View")
	UModioFeaturedCategoryParams* PrimaryCategoryParams;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Featured View")
	TArray<UModioFeaturedCategoryParams*> AdditionalCategoryParams;
};
