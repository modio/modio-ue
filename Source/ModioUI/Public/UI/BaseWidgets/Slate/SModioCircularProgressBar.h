#pragma once

#include "Templates/SharedPointer.h"
#include "UI/BaseWidgets/Slate/SModioCircle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class MODIOUI_API SModioCircularProgressBar : public SCompoundWidget
{
protected:
	int32 Resolution;
	float Radius;
	int32 BackgroundThickness;
	int32 ForegroundThickness;
	int32 StartAngleInDegrees;
	TAttribute<float> Progress;
	TAttribute<FSlateColor> BackgroundColor;
	TAttribute<FSlateColor> ForegroundColor;

	TSharedPtr<SModioCircle> Background;
	TSharedPtr<SModioCircle> Foreground;

	FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	int32 GetProgress() const;

public:
	SLATE_BEGIN_ARGS(SModioCircularProgressBar)
		: _Resolution(100),
		  _Radius(32),
		  _BackgroundThickness(1),
		  _ForegroundThickness(1),
		  _StartAngleInDegrees(0),
		  _Progress(1),
		  _BackgroundColor(FLinearColor::White),
		  _ForegroundColor(FLinearColor::White)
	{}

	SLATE_ARGUMENT(int32, Resolution)
	SLATE_ARGUMENT(float, Radius)
	SLATE_ARGUMENT(int32, BackgroundThickness)
	SLATE_ARGUMENT(int32, ForegroundThickness)
	SLATE_ARGUMENT(int32, StartAngleInDegrees)
	SLATE_ATTRIBUTE(float, Progress)
	SLATE_ATTRIBUTE(FSlateColor, BackgroundColor)
	SLATE_ATTRIBUTE(FSlateColor, ForegroundColor)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetBackgroundThickness(float NewBackgroundThickness);
	void SetForegroundThickness(float NewForegroundThickness);
	void SetRadius(float NewRadius);
	
	void SetProgress(TAttribute<float> NewProgress);
	void SetBackgroundColor(FLinearColor NewBackgroundColor);
	void SetForegroundColor(FLinearColor NewForegroundColor);
	void SetStartAngle(int32 NewStartAngle);
};