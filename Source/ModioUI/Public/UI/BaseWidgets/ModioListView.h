// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ListView.h"
#include "CoreMinimal.h"
#include "ModioListView.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioListView : public UListView
{
	GENERATED_BODY()
protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	bool bSimulateSelection = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	int32 SimulatedSelectionIndex = 0;
#endif

	virtual void SynchronizeProperties() override;
};
