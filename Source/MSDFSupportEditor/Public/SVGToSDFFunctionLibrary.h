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
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SVGToSDF.h"

#include "SVGToSDFFunctionLibrary.generated.h"

UCLASS()
class MSDFSUPPORTEDITOR_API USVGToSDFFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "SVGToSDFFunctionLibrary")
	static bool ConvertSVGToMSDFData(const FString& FilePath, const FSVGConversionParams& Params,
									 TArray<uint8>& Output);
};