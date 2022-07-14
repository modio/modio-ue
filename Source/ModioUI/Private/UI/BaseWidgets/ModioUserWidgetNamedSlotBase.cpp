// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioUserWidgetNamedSlotBase.h"
#include "ModioUI.h"

void UModioUserWidgetNamedSlotBase::GetSlotNames(TArray<FName>& SlotNames) const
{
	Super::GetSlotNames(SlotNames);
	TArray<FName> TempSlotNames;
	NativeGetSlotNames(TempSlotNames);
	for (const FName& Name : DesignerSpecifiedSlotNames)
	{
		if (TempSlotNames.Contains(Name))
		{
			UE_LOG(LogModioUI, Warning,
				   TEXT("SlotName %s was already added from code. Please remove it from DesignerSpecifiedSlotNames!"),
				   *Name.ToString());
		}
		else
		{
			SlotNames.Add(Name);
		}
	}
	SlotNames.Append(TempSlotNames);
}

UWidget* UModioUserWidgetNamedSlotBase::GetContentForSlot(FName SlotName) const
{
	if (NamedWidgets.Contains(SlotName))
	{
		return NamedWidgets[SlotName];
	}
	return Super::GetContentForSlot(SlotName);
}

void UModioUserWidgetNamedSlotBase::SetContentForSlot(FName SlotName, UWidget* Content)
{
	TArray<FName> SlotNames;
	GetSlotNames(SlotNames);
	if (SlotNames.Contains(SlotName))
	{
		NamedWidgets.Add(SlotName, Content);
	}
	else
	{
		Super::SetContentForSlot(SlotName, Content);
	}
}
