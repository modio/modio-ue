// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioComboBox.h"
#include "Types/SlateEnums.h"

void UModioComboBox::SynchronizeProperties()
{
	MyComboBox->SetMenuPlacement(MenuPlacement);
	Super::SynchronizeProperties();
}
