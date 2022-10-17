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

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "SlateMaterialBrush.h"

#include "ModioSlateMaterialBrush.generated.h"

/**
* Struct container for a slate material brush, in other words, A brush which contains 
* information about how to draw a Slate element
**/
USTRUCT(BlueprintType)
struct FModioSlateMaterialBrush : public FSlateBrush
{
	GENERATED_BODY()
	
	/**
	* Default constructor without parameters
	**/
	FModioSlateMaterialBrush() = default;
	
	/**
	* Default destructor without parameters
	**/
	~FModioSlateMaterialBrush() = default;

	/**
	* Convenience constructor with the material and size parameters
	* @param InMaterial Reference interface material to store in this instance
	* @param InMaterialSize 2D vector with the material size
	**/
	FModioSlateMaterialBrush(UMaterialInterface* InMaterial, const FVector2D& InMaterialSize)
		: FSlateBrush(ESlateBrushDrawType::Image, FName(InMaterial->GetFName()), FMargin(0),
					  ESlateBrushTileType::NoTile, ESlateBrushImageType::FullColor, InMaterialSize, FLinearColor::White)
	{
		SetResourceObject(InMaterial);
		// MaterialInstance->AddToRoot();
	}
};