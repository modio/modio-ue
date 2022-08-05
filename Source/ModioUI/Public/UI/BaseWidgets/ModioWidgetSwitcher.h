// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetSwitcher.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"

#include "ModioWidgetSwitcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIndexChanged, int32, Index);

UCLASS()
class MODIOUI_API UModioWidgetSwitcher : public UWidgetSwitcher
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="Widgets")
	FOnIndexChanged OnActiveWidgetChanged;
	TArray<int32> ActiveWidgetIndexStack;

public:
	virtual void PushActiveWidgetIndex(int32 Index);
	virtual int32 PopActiveWidgetIndex();
	virtual void SetActiveWidgetIndex(int32 Index) override;

	virtual FOnIndexChanged& GetActiveWidgetChangedDelegate()
	{
		return OnActiveWidgetChanged;
	}
};
