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

#include "CoreMinimal.h"
#include "Types/ModioModInfo.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UObject/NoExportTypes.h"

#include "ModioModInfoUI.generated.h"

/**
* Wrapper around a FModioModInfo because UMG widgets expect list items to be UObject-based
**/
UCLASS(BlueprintType)
class MODIOUICORE_API UModioModInfoUI : public UObject, public IModioModInfoUIDetails
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

	/**
	* Stored property to the ModInfo in this class
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioModInfoUI")
	FModioModInfo Underlying;
};
