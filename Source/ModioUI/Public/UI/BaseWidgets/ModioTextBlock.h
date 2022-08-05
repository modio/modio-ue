// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioTextBlock.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioTextBlock : public UTextBlock
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Appearance")
	FModioUIStyleRef StyleRef = {FName("DefaultTextStyle") };

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance")
	bool bOverrideGlobalStyle = false;

	void ApplyStyleOverrides();
	virtual void SynchronizeProperties() override;
};
