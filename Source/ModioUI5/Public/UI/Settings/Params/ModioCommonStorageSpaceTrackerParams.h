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
#include "ModioCommonStorageSpaceTrackerParams.generated.h"

/**
 * Project Settings customization for ModioCommonStorageSpaceTrackerUserWidget
 */
UCLASS(Config = "ModioCommonStorageSpaceTrackerParams", DefaultConfig, meta = (DisplayName = "Storage Space Tracker Params"))
class MODIOUI5_API UModioCommonStorageSpaceTrackerParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText UsedSpaceLabelText = NSLOCTEXT("Modio", "UsedSpace", "Used space");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText FreeSpaceLabelText = NSLOCTEXT("Modio", "FreeSpace", "Free space");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TotalSpaceLabelText = NSLOCTEXT("Modio", "TotalSpace", "Total space");

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};
