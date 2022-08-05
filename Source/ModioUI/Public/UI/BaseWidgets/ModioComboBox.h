// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ComboBox.h"

#include "ModioComboBox.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUI_API UModioComboBox : public UComboBox
{
	GENERATED_BODY()
	
	protected:

	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TEnumAsByte<EMenuPlacement> MenuPlacement;
};
