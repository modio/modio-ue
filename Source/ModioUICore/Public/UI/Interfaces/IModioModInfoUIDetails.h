/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Types/ModioModInfo.h"
#include "UObject/Interface.h"

#include "IModioModInfoUIDetails.generated.h"

/**
 *
 */
UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioModInfoUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioModInfoUIDetails : public IInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModInfoUIDetails")
	FModioModInfo GetFullModInfo();
	virtual FModioModInfo GetFullModInfo_Implementation()
	{
		return FModioModInfo();
	};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IModioModInfoUIDetails")
	FModioModID GetModID();
	virtual FModioModID GetModID_Implementation()
	{
		return FModioModID();
	}
};