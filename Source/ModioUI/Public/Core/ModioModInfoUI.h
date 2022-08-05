// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/ModioModInfo.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UObject/NoExportTypes.h"

#include "ModioModInfoUI.generated.h"

/**
 * Wrapper around a FModioModInfo because UMG widgets expect list items to be UObject-based
 */
UCLASS(BlueprintType)
class MODIOUI_API UModioModInfoUI : public UObject, public IModioModInfoUIDetails
{
	GENERATED_BODY()
public:
	virtual FModioModInfo GetFullModInfo_Implementation() override
	{
		return Underlying;
	}

	virtual FModioModID GetModID_Implementation() override
	{
		return Underlying.ModId;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ModioModInfoUI")
	FModioModInfo Underlying;
};
