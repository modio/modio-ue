/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "SVGToSDF.h"
#include "MSDFGenWrapper.h"

bool USVGToSDFFunctionLibrary::ConvertSVGToMSDFData(const FString& FilePath, const FSVGConversionParams& Params,
													TArray<uint8>& Output)
{
	msdfgen::Shape SVGShape;
	msdfgen::Vector2 OutputDimensions = msdfgen::Vector2(Params.OutputDimensions.X, Params.OutputDimensions.Y);
	if (!msdfgen::loadSvgShape(SVGShape, TCHAR_TO_ANSI(*FilePath), Params.SVGPathIndex, &OutputDimensions))
	{
		return false;
	}

	SVGShape.normalize();
	msdfgen::edgeColoringByDistance(SVGShape, 3.0);
	msdfgen::Bitmap<float, 3> msdf(Params.OutputDimensions.X, Params.OutputDimensions.Y);
	msdfgen::Projection Transform(msdfgen::Vector2(Params.Scale.X, Params.Scale.Y),
								  msdfgen::Vector2(Params.Translation.X, Params.Translation.Y));
	float RangeInShapeUnits = 2;
	if (Params.bAutoFrame)
	{
		msdfgen::Shape::Bounds bounds = SVGShape.getBounds();
		float BoundsWidth = bounds.r - bounds.l;
		float BoundsHeight = bounds.t - bounds.b;

		msdfgen::Vector2 Scale = msdfgen::Vector2((Params.OutputDimensions.X) / (BoundsWidth + Params.ExtraMargin),
												  (Params.OutputDimensions.Y) / (BoundsHeight + Params.ExtraMargin));
		msdfgen::Vector2 Translation =
			.5 * (msdfgen::Vector2(Params.OutputDimensions.X, Params.OutputDimensions.Y) / Scale -
				  msdfgen::Vector2(BoundsWidth, BoundsHeight)) -
			msdfgen::Vector2(bounds.l, bounds.b);
		Transform = msdfgen::Projection(Scale, Translation);
		RangeInShapeUnits = Params.FieldRangeInPixels / FMath::Min(Scale.x, Scale.y);
	}
	else
	{
		RangeInShapeUnits = Params.FieldRangeInPixels / FMath::Min(Params.Scale.X, Params.Scale.Y);
	}

	msdfgen::generateMSDF(
		msdf, SVGShape, Transform, RangeInShapeUnits,
		msdfgen::MSDFGeneratorConfig(
			true, msdfgen::ErrorCorrectionConfig(msdfgen::ErrorCorrectionConfig::EDGE_PRIORITY,
												 msdfgen::ErrorCorrectionConfig::ALWAYS_CHECK_DISTANCE)));
	Output.Reserve(4 * msdf.width() * msdf.height());
	// msdfgen has positive height values going upwards rather than downwards
	for (int32 CurrentY = msdf.height() - 1; CurrentY >= 0; CurrentY--)
	{
		for (int32 CurrentX = 0; CurrentX < msdf.width(); CurrentX++)
		{
			/// B8G8R8A8
			Output.Add(msdfgen::pixelFloatToByte(msdf(CurrentX, CurrentY)[2]));
			Output.Add(msdfgen::pixelFloatToByte(msdf(CurrentX, CurrentY)[1]));
			Output.Add(msdfgen::pixelFloatToByte(msdf(CurrentX, CurrentY)[0]));
			Output.Add(msdfgen::pixelFloatToByte(1.0f));
		}
	}
	return true;
}
