// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Delegates/DelegateCombinations.h"

#include "ModioUIAction.generated.h"

DECLARE_DYNAMIC_DELEGATE(FModioUIExecuteAction);

DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FModioUICanExecuteAction);

DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FModioUIIsActionButtonVisible);

/**
 * 
 */
USTRUCT(BlueprintType)
struct MODIOUI_API FModioUIAction
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UIActions")
	FModioUIExecuteAction ExecuteAction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UIActions")
	FModioUICanExecuteAction CanExecuteAction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UIActions")
	FModioUIIsActionButtonVisible IsActionVisible;
};
