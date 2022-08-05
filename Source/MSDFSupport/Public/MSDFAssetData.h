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
#include "Engine/AssetUserData.h"
#include "SVGToSDF.h"

#include "MSDFAssetData.generated.h"

/**
 *
 */
UCLASS()
class MSDFSUPPORT_API UMSDFAssetData : public UAssetUserData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Params")
	FSVGConversionParams ImportParameters;
};
