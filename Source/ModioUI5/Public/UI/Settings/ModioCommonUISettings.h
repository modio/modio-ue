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

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UI/Settings/ModioCommonDefines.h"

#include "ModioCommonUISettings.generated.h"

/**
 * General settings for Mod.io Common UI
 */
UCLASS(Config = "ModioCommonUISettings", DefaultConfig)
class MODIOUI5_API UModioCommonUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Enables a user interface for enabling and disabling mods  */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Settings")
	bool bEnableCollectionModDisableUI = false;
	
	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};
