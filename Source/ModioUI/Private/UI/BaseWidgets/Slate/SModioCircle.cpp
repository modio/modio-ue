#include "UI/BaseWidgets/Slate/SModioCircle.h"
#include "Math/UnrealMathUtility.h"
#include "Rendering/DrawElements.h"

int32 SModioCircle::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
							FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
							bool bParentEnabled) const
{
	float StartOffsetInRadians = FMath::DegreesToRadians(-90 + StartAngle);
	float AngleStepInRadians = FMath::DegreesToRadians(360.f / Resolution);
	TArray<FVector2D> Points;
	float CurrentAngleInRadians = 0;
	float ArcSizeInRadians = FMath::DegreesToRadians(FMath::Clamp(Arc.Get(), 0, 360));
	while (CurrentAngleInRadians < ArcSizeInRadians)
	{
		Points.Add(FVector2D(FMath::Cos(CurrentAngleInRadians + StartOffsetInRadians),
							 FMath::Sin(CurrentAngleInRadians + StartOffsetInRadians)) *
				   Radius);
		CurrentAngleInRadians += AngleStepInRadians;
	}
	Points.Add(FVector2D(FMath::Cos(ArcSizeInRadians + StartOffsetInRadians),
						 FMath::Sin(ArcSizeInRadians + StartOffsetInRadians)) *
			   Radius);
	// Basically we assume if you have arc set to 360 you want a closed shape, and otherwise not
	/*
		if (FMath::IsNearlyEqual(Arc, 360.f))
		{
			Points.Add(FVector2D(FMath::Cos(0), FMath::Sin(0)) * Radius);
		}*/

	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId,
		AllottedGeometry.ToPaintGeometry(FSlateLayoutTransform(AllottedGeometry.GetLocalSize() / 2)), Points,
		ESlateDrawEffect::None, ColorAndOpacity.Get().GetSpecifiedColor(), true, Thickness * AllottedGeometry.Scale);
	return LayerId;
}

FVector2D SModioCircle::ComputeDesiredSize(float) const
{
	return FVector2D(Thickness + Radius * 2);
}