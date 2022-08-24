// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ProgressBar.h"
#include "CoreMinimal.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Misc/EngineVersionComparison.h"

#if UE_VERSION_NEWER_THAN(5, 0, 0)
	#include "UObject/ObjectSaveContext.h"
#endif

#include "ModioProgressBar.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioProgressBar : public UProgressBar
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioProgressBarStyle"), Category="Widget")
	FModioUIStyleRef StyleRef;

	virtual void SynchronizeProperties() override;
#if UE_VERSION_NEWER_THAN(5, 0, 0)
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#else
	virtual void PreSave(const class ITargetPlatform* TargetPlatform) override;
#endif

public:
	UFUNCTION(BlueprintCallable, Category="ModioProgressBar")
	void SetStyleRef(FModioUIStyleRef NewStyle);
};
