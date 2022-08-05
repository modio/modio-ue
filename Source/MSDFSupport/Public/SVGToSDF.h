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
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SVGToSDF.generated.h"

USTRUCT(BlueprintType)
struct MSDFSUPPORT_API FSVGConversionParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SVGConversionParams")
	int32 SVGPathIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPreserveRatio), Category = "SVGConversionParams")
	FVector2D OutputDimensions = FVector2D(32, 32);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SVGConversionParams")
	bool bAutoFrame = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "AutoFrame"), Category = "SVGConversionParams")
	int32 ExtraMargin = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!bAutoFrame"), meta = (AllowPreserveRatio),
			  Category = "SVGConversionParams")
	FVector2D Scale = FVector2D(1, 1);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!bAutoFrame"),
			  Category = "SVGConversionParams")
	FVector2D Translation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SVGConversionParams")
	int32 FieldRangeInPixels = 8;

	// For future use
	// UPROPERTY(BlueprintReadWrite,EditAnywhere)
	// bool bIncludeSdfInAlpha = false;
};

UCLASS()
class MSDFSUPPORT_API USVGToSDFFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="SVGToSDFFunctionLibrary")
	static bool ConvertSVGToMSDFData(const FString& FilePath, const FSVGConversionParams& Params,
									 TArray<uint8>& Output);
};