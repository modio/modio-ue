/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Types/ModioFilterParams.h"
#include "UObject/NoExportTypes.h"

#include "ModioFeaturedCategoryParams.generated.h"

/**
* Class that stores mod attributes like category name or tags
**/
UCLASS(EditInlineNew)
class MODIOUICORE_API UModioFeaturedCategoryParams : public UObject
{
	GENERATED_BODY()
public:
	/**
	* Stored property of the text to use as the category name
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFeaturedCategoryParams")
	FText CategoryName;

	/**
	* Stored property array to a list of tags for the category
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFeaturedCategoryParams")
	TArray<FString> Tags;

	/**
	* Stored property array to a list of excluded tags for the category
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFeaturedCategoryParams")
	TArray<FString> ExcludedTags;

	/**
	* Stored property of the sort direction to use when displaying elements
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFeaturedCategoryParams")
	EModioSortDirection Direction;

	/**
	* Stored property of the type to use as sort field
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFeaturedCategoryParams")
	EModioSortFieldType SortField;

	/**
	* Stored property of the featured categories count
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFeaturedCategoryParams")
	int64 Count = 20;
};
