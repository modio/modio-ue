// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Border.h"
#include "CoreMinimal.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"

#include "ModioAsyncOpWrapperWidget.generated.h"

/**
 * Simple wrapper so an arbitrary widget can be used as the loaded-state widget in an AsyncLoader
 * Only really used to work around an issue in the details view at the moment
 */
UCLASS()
class MODIOUI_API UModioAsyncOpWrapperWidget : public UBorder, public IModioUIAsyncOperationWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TScriptInterface<IModioUIAsyncOperationWidget> ActualWidget;

	void NativeRequestOperationRetry() override;

	void NativeNotifyOperationState(EModioUIAsyncOperationWidgetState NewState) override;

public:
	UFUNCTION(BlueprintCallable, Category="ModioAsyncOpWrapperWidget")
	void SetActualAsyncOperationWidget(const TScriptInterface<IModioUIAsyncOperationWidget>& Widget);
};
