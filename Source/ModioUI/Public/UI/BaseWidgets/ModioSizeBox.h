// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SizeBox.h"
#include "ModioSizeBox.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUI_API UModioSizeBox : public USizeBox
{
	GENERATED_BODY()
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Content")
	TEnumAsByte<EHorizontalAlignment> HAlign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Content")
	TEnumAsByte<EVerticalAlignment> VAlign;
};
