// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioRetainerBox.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioRoundedBorder.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioRoundedBorder : public UModioRetainerBox
{
	GENERATED_BODY()
	virtual void SynchronizeProperties() override;

public:
	// TODO: make this a config var, then it can be overridden in the instances
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	TSoftObjectPtr<UMaterialInterface> RetainerMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRoundedBorderStyle"), Category="Widgets")
	FModioUIStyleRef BorderStyle;

/*
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	bool bEnableBorder = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	FLinearColor BorderColor = FColor::White;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	float BorderRadius = 48;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	float BorderThickness = 4;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	float BorderOpacity = 1;*/

	void SetHoveredState();
	void ClearHoveredState();
	void EnableBorder();
	void DisableBorder();
	// BorderColour
	// radius
	// thickness
	// call GetEffectMaterial and set the parameters
};
