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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 10, ClampMax = 1000), Category="Widgets")
	float Resolution = 60;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0.0, ClampMax = 360.0), Category="Widgets")
	float StartAngleInDegrees = 0;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0.0, ClampMax = 360.0), Category="Widgets")
	float ArcInDegrees = 360;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1), Category="Widgets")
	float Radius = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1), Category="Widgets")
	float Thickness = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FSlateColor ColorAndOpacity = FLinearColor::White;

public:
	void SynchronizeProperties() override;
	void ReleaseSlateResources(bool bReleaseChildren) override;
};
