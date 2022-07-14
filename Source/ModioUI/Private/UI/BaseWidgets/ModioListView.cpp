// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioListView.h"

void UModioListView::SynchronizeProperties()
{
	Super::SynchronizeProperties();
#if WITH_EDITORONLY_DATA
	if (IsDesignTime() && bSimulateSelection)
	{
		SetSelectedIndex(SimulatedSelectionIndex);
	}
#endif
}
