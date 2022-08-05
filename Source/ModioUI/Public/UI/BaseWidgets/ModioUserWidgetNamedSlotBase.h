// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/NamedSlotInterface.h"
#include "CoreMinimal.h"
#include "ModioUserWidgetBase.h"

#include "ModioUserWidgetNamedSlotBase.generated.h"

/**
 *
 */
UCLASS(abstract)
class MODIOUI_API UModioUserWidgetNamedSlotBase : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	
	UPROPERTY()
	TMap<FName, UWidget*> NamedWidgets;

	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category="Widgets")
	TArray<FName> DesignerSpecifiedSlotNames;

	// UNamedSlotInterface Begin
	virtual void GetSlotNames(TArray<FName>& SlotNames) const override;
	virtual UWidget* GetContentForSlot(FName SlotName) const override;
	virtual void SetContentForSlot(FName SlotName, UWidget* Content) override;
	// UNamedSlotInterface End

	virtual void NativeGetSlotNames(TArray<FName>& SlotNames) const {};
};
