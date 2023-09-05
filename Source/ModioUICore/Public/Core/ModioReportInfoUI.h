/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Core/ModioModInfoUI.h"
#include "CoreMinimal.h"
#include "Types/ModioReportParams.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UObject/Object.h"

#include "ModioReportInfoUI.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioReportInfoUI : public UObject, public IModioModInfoUIDetails
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
