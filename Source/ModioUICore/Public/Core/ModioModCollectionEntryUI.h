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
#include "Types/ModioModCollectionEntry.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"

#include "ModioModCollectionEntryUI.generated.h"

/**
* Wrapper around FModioModCollectionEntry because UMG widgets expect list items to be UObject-based
**/
UCLASS(BlueprintType)
class MODIOUICORE_API UModioModCollectionEntryUI : public UObject, public IModioModInfoUIDetails
{
	GENERATED_BODY()

public:
	/**
	* Stored property to the collection entry strong type
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "ModioModCollectionEntryUI")
	FModioModCollectionEntry Underlying;

	/**
	* Stored property boolean to signal if subscriptions are already cached, false by default
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "ModioModCollectionEntryUI")
	bool bCachedSubscriptionStatus = false;

	virtual FModioModInfo GetFullModInfo_Implementation() override
	{
		return Underlying.GetModProfile();
	}

	virtual FModioModID GetModID_Implementation() override
	{
		return Underlying.GetID();
	}
};
