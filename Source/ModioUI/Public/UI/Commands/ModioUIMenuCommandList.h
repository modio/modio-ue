// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Commands/ModioUIAction.h"
#include "UI/Commands/ModioUIMenuEntry.h"
#include "UObject/NoExportTypes.h"

#include "ModioUIMenuCommandList.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct MODIOUI_API FModioUIMenuCommandList
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|Menu Commands")
	TMap<FModioUIMenuEntry, FModioUIAction> MappedActions;
};
