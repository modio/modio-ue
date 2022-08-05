// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Types/ModioFilterParams.h"

#include "ModioFeaturedCategoryParams.generated.h"


/**
 *
 */
UCLASS(EditInlineNew)
class MODIOUI_API UModioFeaturedCategoryParams : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ModioFeaturedCategoryParams")
	FText CategoryName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFeaturedCategoryParams")
	TArray<FString> Tags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFeaturedCategoryParams")
	TArray<FString> ExcludedTags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFeaturedCategoryParams")
	EModioSortDirection Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFeaturedCategoryParams")
	EModioSortFieldType SortField;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFeaturedCategoryParams")
	int64 Count = 20;
};
