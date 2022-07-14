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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText CategoryName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> Tags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> ExcludedTags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EModioSortDirection Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EModioSortFieldType SortField;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int64 Count = 20;
};
