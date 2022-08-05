// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Types/ModioModInfo.h"
#include "UObject/Interface.h"
#include "IModioModInfoUIDetails.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType)
class MODIOUI_API UModioModInfoUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioModInfoUIDetails : public IInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent , Category="IModioModInfoUIDetails")
	FModioModInfo GetFullModInfo();
	virtual FModioModInfo GetFullModInfo_Implementation()
	{
		return FModioModInfo();
	};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="IModioModInfoUIDetails")
	FModioModID GetModID();
	virtual FModioModID GetModID_Implementation()
	{
		return FModioModID();
	}
};