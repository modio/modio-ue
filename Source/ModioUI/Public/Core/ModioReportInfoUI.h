// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/ModioModInfoUI.h"
#include "Types/ModioReportParams.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UObject/Object.h"

#include "ModioReportInfoUI.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioReportInfoUI : public UObject, public IModioModInfoUIDetails
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FModioReportParams ReportData;

	UPROPERTY()
	UObject* ModInfo;

	virtual FModioModInfo GetFullModInfo_Implementation() override
	{
		if (ModInfo->Implements<UModioModInfoUIDetails>())
		{
			return IModioModInfoUIDetails::Execute_GetFullModInfo(ModInfo);
		}

		return FModioModInfo();
	}

	virtual FModioModID GetModID_Implementation() override
	{
		if (ModInfo->Implements<UModioModInfoUIDetails>())
		{
			return IModioModInfoUIDetails::Execute_GetModID(ModInfo);
		}

		return FModioModID();
	}
};
