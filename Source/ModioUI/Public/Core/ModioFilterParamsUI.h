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

#include "ModioFilterParamsUI.generated.h"

/**
* Wrapper around FModioFilterParams because UMG widgets expect UObjects for data sources or list items
**/
UCLASS(BlueprintType)
class MODIOUI_API UModioFilterParamsUI : public UObject
{
	GENERATED_BODY()

public:
	/**
	* Stored property of the filter params in this class
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFilterParamsUI")
	FModioFilterParams Underlying;
};
