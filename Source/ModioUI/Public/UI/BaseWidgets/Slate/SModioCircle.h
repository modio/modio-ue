#pragma once

#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SLeafWidget.h"

class MODIOUI_API SModioCircle : public SLeafWidget
{
public:
	int32 Resolution;
	float Radius;
	int32 Thickness;
	TAttribute<int32> Arc;
	int32 StartAngle;
	TAttribute<FSlateColor> ColorAndOpacity;

	SLATE_BEGIN_ARGS(SModioCircle) : _Resolution(100), _Radius(32), _Thickness(1), _Arc(360), _StartAngle(0) {}

	SLATE_ARGUMENT(int32, Resolution)
	SLATE_ARGUMENT(float, Radius)
	SLATE_ARGUMENT(int32, Thickness)
	SLATE_ATTRIBUTE(int32, Arc)
	SLATE_ARGUMENT(int32, StartAngle)
	SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		Resolution = InArgs._Resolution;
		Radius = InArgs._Radius;
		Thickness = InArgs._Thickness;
		Arc = InArgs._Arc;
		StartAngle = InArgs._StartAngle;
		ColorAndOpacity = InArgs._ColorAndOpacity;
	}

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
						  FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
						  bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
};