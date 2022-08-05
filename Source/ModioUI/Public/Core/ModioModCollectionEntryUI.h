// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/ModioModCollectionEntry.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"

#include "ModioModCollectionEntryUI.generated.h"

/**
 * Wrapper around a FModioModCollectionEntry because UMG widgets expect list items to be UObject-based
 */
UCLASS(BlueprintType)
class MODIOUI_API UModioModCollectionEntryUI : public UObject, public IModioModInfoUIDetails
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category="ModioModCollectionEntryUI")
	FModioModCollectionEntry Underlying;

	/// @brief Is this entry in the current user's subscriptions?
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient,  Category="ModioModCollectionEntryUI")
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
