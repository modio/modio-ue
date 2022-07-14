// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ProgressBar.h"
#include "CoreMinimal.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioProgressBar.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioProgressBar : public UProgressBar
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioProgressBarStyle"))
	FModioUIStyleRef StyleRef;

	virtual void SynchronizeProperties() override;
	virtual void PreSave(const class ITargetPlatform* TargetPlatform) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetStyleRef(FModioUIStyleRef NewStyle);
};
