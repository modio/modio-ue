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
#include "Materials/MaterialExpression.h"

#include "RenderMSDF.generated.h"

/**
 *
 */
UCLASS()
class MSDFSUPPORT_API URenderMSDF : public UMaterialExpression
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UTexture* EffectiveTexture = nullptr;
#if WITH_EDITOR
	int32 CompileInternal(class FMaterialCompiler* Compiler, int32 OutputIndex);
#endif
	UPROPERTY(meta = (DisplayName = "Input MSDF Texture"))
	FExpressionInput MSDFTexture;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Texture")
	TEnumAsByte<enum EMaterialSamplerType> SamplerType;

	UPROPERTY()
	FExpressionInput Coordinates;

	/** only used if Coordinates is not hooked up */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	uint8 ConstCoordinateIndex;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionTextureBase)
	class UTexture* Texture;

	UPROPERTY()
	FExpressionInput FieldDistance;

	UPROPERTY()
	FExpressionInput OutputTextureSize;

	UPROPERTY()
	FExpressionOutput Output;

protected:
#if WITH_EDITOR
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
#endif
	virtual UObject* GetReferencedTexture() const override;

	virtual bool CanReferenceTexture() const override
	{
		return true;
	}
#if WITH_EDITOR
	virtual uint32 GetInputType(int32 InputIndex) override;

	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
#endif
};
