// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "CoreMinimal.h"

#include "ModioCircle.generated.h"

class SModioCircle;

/**
 *
 */
UCLASS()
class MODIOUI_API UModioCircle : public UWidget
{
	GENERATED_BODY()
protected:
	TSharedRef<SWidget> RebuildWidget() override;
	TSharedPtr<SModioCircle> MyCircle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 10, ClampMax = 1000))
	float Resolution = 60;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0.0, ClampMax = 360.0))
	float StartAngleInDegrees = 0;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0.0, ClampMax = 360.0))
	float ArcInDegrees = 360;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1))
	float Radius = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1))
	float Thickness = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSlateColor ColorAndOpacity = FLinearColor::White;

public:
	void SynchronizeProperties() override;
	void ReleaseSlateResources(bool bReleaseChildren) override;
};
