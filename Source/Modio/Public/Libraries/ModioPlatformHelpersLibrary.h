/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModioPlatformHelpersLibrary.generated.h"

/**
 * @docpublic
 * @brief Blueprint library which contains a variety of Platform-based helpers for mapping
 * initialization functionality (ie Auth, Portal, Platform etc) based off Platform.
 */
UCLASS()
class MODIO_API UModioPlatformHelpersLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Gets the current platform that the game is running on
	 * @return EModioPlatformName of the current platform
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Platform Helpers")
	static EModioPlatformName GetCurrentPlatform();

	/**
	 * @docpublic
	 * @brief Get the default portal for the platform the game is running on.
	 * @return EModioPortal of the portal to use
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Platform Helpers")
	static EModioPortal GetDefaultPortalForCurrentPlatform();

	/**
	 * @docpublic
	 * @brief Get the default Authentication Provider for the current platform the game is running on
	 * @return EModioAuthenticationProvider to use for authentication calls
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Platform Helpers")
	static EModioAuthenticationProvider GetDefaultAuthProviderForCurrentPlatform();
};
