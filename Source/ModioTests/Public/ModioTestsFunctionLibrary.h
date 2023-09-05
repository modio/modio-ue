// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioInitializeOptions.h"

#include "ModioTestsFunctionLibrary.generated.h"

/**
 *
 */
UCLASS(MinimalAPI)
class UModioTestsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
#if WITH_DEV_AUTOMATION_TESTS
	/// @brief Gets the automation test options. not exposed to BP to prevent accidental use by developers in BP
	static MODIOTESTS_API FModioInitializeOptions GetAutomationTestOptions();
	/// @brief Gets the currently hard-coded mod ID for testing subscriptions and unsubscriptions
	/// @return the ID
	static MODIOTESTS_API struct FModioModID GetTestModID();
#endif
};
