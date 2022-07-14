// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioCircle.h"
#include "UI/BaseWidgets/Slate/SModioCircle.h"

void UModioCircle::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (MyCircle)
	{
		MyCircle->Arc = ArcInDegrees;
		MyCircle->Radius = Radius;
		MyCircle->Resolution = Resolution;
		MyCircle->Thickness = Thickness;
		MyCircle->StartAngle = StartAngleInDegrees;
		MyCircle->ColorAndOpacity = ColorAndOpacity;
	}
}

void UModioCircle::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyCircle.Reset();
}

TSharedRef<SWidget> UModioCircle::RebuildWidget()
{
	return SAssignNew(MyCircle, SModioCircle)
		.Arc(ArcInDegrees)
		.StartAngle(StartAngleInDegrees)
		.Radius(Radius)
		.Resolution(Resolution)
		.Thickness(Thickness)
		.ColorAndOpacity(ColorAndOpacity);
}
