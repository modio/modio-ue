// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioCircularProgressBar.h"
#include "Components/Widget.h"

UModioCircularProgressBar::FGetFloat& UModioCircularProgressBar::GetProgressDelegate()
{
	return OnGetProgress;
}


void UModioCircularProgressBar::SetProgress(float NewValue) 
{
	Progress = NewValue;
}

void UModioCircularProgressBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyProgressBar->SetBackgroundThickness(BackgroundThickness);
	MyProgressBar->SetForegroundThickness(ForegroundThickness);
	MyProgressBar->SetRadius(Radius);
	MyProgressBar->SetBackgroundColor(BackgroundColor);
	MyProgressBar->SetForegroundColor(ForegroundColor);
	MyProgressBar->SetStartAngle(StartAngleInDegrees);
}

void UModioCircularProgressBar::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyProgressBar.Reset();
}

TSharedRef<SWidget> UModioCircularProgressBar::RebuildWidget()
{
	return SAssignNew(MyProgressBar, SModioCircularProgressBar)
		.BackgroundThickness(BackgroundThickness)
		.ForegroundThickness(ForegroundThickness)
		.Radius(Radius)
		.Progress_UObject(this, &UModioCircularProgressBar::GetProgress)
		.BackgroundColor(BackgroundColor)
		.ForegroundColor(ForegroundColor)
		.StartAngleInDegrees(StartAngleInDegrees);
}

float UModioCircularProgressBar::GetProgress_Implementation() const
{
	if (OnGetProgress.IsBound())
	{
		return OnGetProgress.Execute();
	}
	else
	{
		return Progress;
	}
}
