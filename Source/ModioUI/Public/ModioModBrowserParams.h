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
#include "Engine/DataAsset.h"
#include "UI/Views/CategoryBrowser/ModioFeaturedCategoryParams.h"
#include "UObject/NoExportTypes.h"

#include "ModioModBrowserParams.generated.h"

/**
* Modio element that stores category parameters for the Modio browser
**/
UCLASS(BlueprintType)
class MODIOUI_API UModioModBrowserParams : public UDataAsset
{
	GENERATED_BODY()
public:
	/**
	* Mod browser params default constructor
	**/
	UModioModBrowserParams();

	/**
	* Stored property reference to the main category param class
	**/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Featured View")
	UModioFeaturedCategoryParams* PrimaryCategoryParams;

	/**
	* Stored property array of pointers to secondary category params
	**/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Featured View")
	TArray<UModioFeaturedCategoryParams*> AdditionalCategoryParams;
};
