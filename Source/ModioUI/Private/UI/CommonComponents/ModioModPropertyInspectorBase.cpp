// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioModPropertyInspectorBase.h"
#include "Core/ModioModInfoUI.h"

void UModioModPropertyInspectorBase::SynchronizeProperties()
{
	UModioUserWidgetBase::SynchronizeProperties();
	if (UModioModInfoUI* UnderlyingData = Cast<UModioModInfoUI>(DataSource))
	{
		UpdatePropertyDisplay();
	}
}

void UModioModPropertyInspectorBase::NativeOnSetDataSource()
{
	UModioUserWidgetBase::NativeOnSetDataSource();
	if (UModioModInfoUI* UnderlyingData = Cast<UModioModInfoUI>(DataSource))
	{
		UpdatePropertyDisplay();
	}
}

void UModioModPropertyInspectorBase::UpdatePropertyDisplay()
{
	bRoutedUpdatePropertyDisplay = false;
	NativeOnUpdatePropertyDisplay();
	ensureMsgf(bRoutedUpdatePropertyDisplay, TEXT("Call Super::NativeOnUpdatePropertyDisplay in your overridden "
												  "implementation to ensure events are routed to blueprint"));
}

void UModioModPropertyInspectorBase::NativeOnUpdatePropertyDisplay()
{
	OnUpdatePropertyDisplay();
	bRoutedUpdatePropertyDisplay = true;
}
