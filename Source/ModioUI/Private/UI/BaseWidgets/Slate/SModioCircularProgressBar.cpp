#include "UI/BaseWidgets/Slate/SModioCircularProgressBar.h"
#include "Math/UnrealMathUtility.h"
#include "Widgets/SOverlay.h"

FVector2D SModioCircularProgressBar::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return SCompoundWidget::ComputeDesiredSize(LayoutScaleMultiplier);
}

void SModioCircularProgressBar::Construct(const FArguments& InArgs)
{
	Resolution = InArgs._Resolution;
	Radius = InArgs._Radius;
	BackgroundThickness = InArgs._BackgroundThickness;
	ForegroundThickness = InArgs._ForegroundThickness;
	Progress = InArgs._Progress;
	BackgroundColor = InArgs._BackgroundColor;
	ForegroundColor = InArgs._ForegroundColor;
	StartAngleInDegrees = InArgs._StartAngleInDegrees;

	// clang-format off
	this->ChildSlot
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			[
				SAssignNew(Background, SModioCircle)
				.Resolution(Resolution)
				.Radius(Radius)
				.Thickness(BackgroundThickness)
				.Arc(360)
				.ColorAndOpacity(BackgroundColor)
				.StartAngle(StartAngleInDegrees)
			]
			+SOverlay::Slot()
			[
				SAssignNew(Foreground, SModioCircle)
				.Resolution(Resolution)
				.Radius(Radius)
				.Thickness(ForegroundThickness)
				.Arc(this, &SModioCircularProgressBar::GetProgress)
				.ColorAndOpacity(ForegroundColor)
				.StartAngle(StartAngleInDegrees)
			]
		];
	// clang-format on
}
int32 SModioCircularProgressBar::GetProgress() const
{
	return Progress.Get() * 360;
}
void SModioCircularProgressBar::SetBackgroundThickness(float NewBackgroundThickness)
{
	if (BackgroundThickness != NewBackgroundThickness)
	{
		BackgroundThickness = NewBackgroundThickness;
		Background->Thickness = BackgroundThickness;
		// Need to trigger a repaint as well as a size calculation
		Invalidate(EInvalidateWidgetReason::Paint | EInvalidateWidgetReason::LayoutAndVolatility);
	}
}

void SModioCircularProgressBar::SetForegroundThickness(float NewForegroundThickness)
{
	if (ForegroundThickness != NewForegroundThickness)
	{
		ForegroundThickness = NewForegroundThickness;
		Foreground->Thickness = ForegroundThickness;
		// Need to trigger a repaint as well as a size calculation
		Invalidate(EInvalidateWidgetReason::Paint | EInvalidateWidgetReason::LayoutAndVolatility);
	}
}

void SModioCircularProgressBar::SetRadius(float NewRadius)
{
	if (Radius != NewRadius)
	{
		Radius = NewRadius;
		Background->Radius = Radius;
		Foreground->Radius = Radius;
		// Need to trigger a repaint as well as a size calculation
		Invalidate(EInvalidateWidgetReason::Paint | EInvalidateWidgetReason::LayoutAndVolatility);
	}
}

void SModioCircularProgressBar::SetProgress(TAttribute<float> NewProgress)
{
	if (Foreground && Progress.Get() != NewProgress.Get())
	{
		Progress = NewProgress;
		Foreground->Arc = 360 * FMath::Clamp(NewProgress.Get(), 0.f, 1.f);
		// Need to trigger a repaint but no need to resize
		Invalidate(EInvalidateWidgetReason::PaintAndVolatility);
	}
}

void SModioCircularProgressBar::SetBackgroundColor(FLinearColor NewBackgroundColor)
{
	if (BackgroundColor.Get() != NewBackgroundColor)
	{
		BackgroundColor = NewBackgroundColor;
		Background->ColorAndOpacity = BackgroundColor;
		// Repaint but dont resize
		Invalidate(EInvalidateWidgetReason::PaintAndVolatility);
	}
}

void SModioCircularProgressBar::SetForegroundColor(FLinearColor NewForegroundColor)
{
	if (ForegroundColor.Get() != NewForegroundColor)
	{
		ForegroundColor = NewForegroundColor;
		Foreground->ColorAndOpacity = ForegroundColor;
		// Repaint but dont resize
		Invalidate(EInvalidateWidgetReason::PaintAndVolatility);
	}
}

void SModioCircularProgressBar::SetStartAngle(int32 NewStartAngle)
{
	if (StartAngleInDegrees != NewStartAngle)
	{
		StartAngleInDegrees = NewStartAngle;
		Background->StartAngle = StartAngleInDegrees;
		Foreground->StartAngle = StartAngleInDegrees;
		Invalidate(EInvalidateWidgetReason::PaintAndVolatility);
	}
}
