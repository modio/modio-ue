// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModioPlatformHelpersLibrary.generated.h"

/**
 * Blueprint library which contains a variety of Platform-based helpers for mapping
 * initialization functionality (ie Auth, Portal, Platform etc) based off Platform.
 */
UCLASS()
class MODIO_API UModioPlatformHelpersLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Gets the current platform that the game is running on
	 * @returns EModioPlatformName of the the current platform that the game is running on
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Platform Helpers")
	static EModioPlatformName GetCurrentPlatform();

	/**
	 * @brief Get the default portal for the platform the game is running on.
	 * @return EModioPortal of the portal to use
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Platform Helpers")
	static EModioPortal GetDefaultPortalForCurrentPlatform();

	/**
	 * @brief Get the default Authentication Provider for the current platform the game is running on
	 * @return EModioAuthenticationProvider to use for authentication calls
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Platform Helpers")
	static EModioAuthenticationProvider GetDefaultAuthProviderForCurrentPlatform();
};
