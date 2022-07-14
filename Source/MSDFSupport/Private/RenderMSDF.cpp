/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "RenderMSDF.h"
#include "MaterialCompiler.h"
#include "Engine/Texture.h"

#if WITH_EDITOR
int32 URenderMSDF::CompileInternal(FMaterialCompiler* Compiler, int32 OutputIndex)
{
	EMaterialSamplerType EffectiveSamplerType = SamplerType;

	UMaterialExpression* InputExpression = MSDFTexture.GetTracedInput().Expression;
	int32 TextureReferenceIndex = INDEX_NONE;
	int32 TextureCodeIndex = INDEX_NONE;
	if (InputExpression)
	{
		TextureCodeIndex = MSDFTexture.Compile(Compiler);
	}
	else
	{
		if (!Texture)
		{
			return CompilerError(Compiler, TEXT("A texture value must be provided"));
		}
		TextureCodeIndex =
			Compiler->Texture(Texture, TextureReferenceIndex, EffectiveSamplerType, SSM_FromTextureAsset, TMVM_None);
	}

	if (TextureCodeIndex == INDEX_NONE)
	{
		return CompilerError(Compiler,
							 TEXT("Provide a texture value via the MSDFTexture input or the Texture property"));
	}

	EffectiveTexture = Texture;
	TOptional<FName> EffectiveParameterName;

	int32 TextureSampleIndex = INDEX_NONE;
	if (InputExpression)
	{
		if (!Compiler->GetTextureForExpression(TextureCodeIndex, TextureReferenceIndex, EffectiveSamplerType,
											   EffectiveParameterName))
		{
			return CompilerError(Compiler, TEXT("Tex input requires a texture value"));
		}
		if (TextureReferenceIndex != INDEX_NONE)
		{
			EffectiveTexture = Cast<UTexture>(Compiler->GetReferencedTexture(TextureReferenceIndex));
		}
	}

	if (EffectiveTexture)
	{
		if (TextureCodeIndex != INDEX_NONE)
		{
			const EMaterialValueType TextureType = Compiler->GetParameterType(TextureCodeIndex);
			if (TextureType == MCT_TextureCube && !Coordinates.GetTracedInput().Expression)
			{
				return CompilerError(Compiler, TEXT("UVW input required for cubemap sample"));
			}
			else if (TextureType == MCT_VolumeTexture && !Coordinates.GetTracedInput().Expression)
			{
				return CompilerError(Compiler, TEXT("UVW input required for volume sample"));
			}
			else if (TextureType == MCT_Texture2DArray && !Coordinates.GetTracedInput().Expression)
			{
				return CompilerError(Compiler, TEXT("UVW input required for texturearray sample"));
			}
		}

		int32 CoordinateIndex = Coordinates.GetTracedInput().Expression
									? Coordinates.Compile(Compiler)
									: Compiler->TextureCoordinate(ConstCoordinateIndex, false, false);

		TextureSampleIndex =
			Compiler->TextureSample(TextureCodeIndex, CoordinateIndex, EffectiveSamplerType, Compiler->Constant(0),
									INDEX_NONE, TMVM_None, SSM_FromTextureAsset, TextureReferenceIndex, true);
	}
	if (TextureSampleIndex != INDEX_NONE)
	{
		int32 SampleR = Compiler->ComponentMask(TextureSampleIndex, true, false, false, false);
		int32 SampleG = Compiler->ComponentMask(TextureSampleIndex, false, true, false, false);
		int32 SampleB = Compiler->ComponentMask(TextureSampleIndex, false, false, true, false);

		int32 MedianRGBValue =
			Compiler->Max(Compiler->Min(SampleR, SampleG), Compiler->Min(Compiler->Max(SampleR, SampleG), SampleB));

		int32 InputTextureDimensions = Compiler->TextureProperty(TextureCodeIndex, TMTM_TextureSize);
		int32 SmallerInputDimension =
			Compiler->Min(Compiler->ComponentMask(InputTextureDimensions, true, false, false, false),
						  Compiler->ComponentMask(InputTextureDimensions, false, true, false, false));
		// Currently assumes a square output
		// Range scaled by the ratio of output texture vs the input texture

		// this should handle the situation such that the field distance is scaled up by the ratio of input image to
		// output image resolution
		int32 RangeScale = Compiler->Mul(Compiler->Div(OutputTextureSize.Compile(Compiler), SmallerInputDimension),
										 FieldDistance.Compile(Compiler));

		int32 UnscaledResult = Compiler->Sub(MedianRGBValue, Compiler->Constant(0.5f));

		int32 UnclampedResult = Compiler->Mul(UnscaledResult, RangeScale);

		int32 ClampedResult = Compiler->Clamp(Compiler->Add(UnclampedResult, Compiler->Constant(0.5f)),
											  Compiler->Constant(0), Compiler->Constant(1));

		return ClampedResult;
	}
	else
	{
		return INDEX_NONE;
	}
}

int32 URenderMSDF::Compile(FMaterialCompiler* Compiler, int32 OutputIndex)
{
	return CompileInternal(Compiler, OutputIndex);
}
#endif
UObject* URenderMSDF::GetReferencedTexture() const
{
	return Texture;
}
#if WITH_EDITOR
uint32 URenderMSDF::GetInputType(int32 InputIndex)
{
	if (InputIndex == 0)
	{
		return MCT_Texture2D;
	}
	else
	{
		return Super::GetInputType(InputIndex);
	}
}

void URenderMSDF::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(FString("Sample MSDF"));
}
#endif