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
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonStorageSpaceTrackerParamsSettings
{
	GENERATED_BODY()

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText UsedSpaceLabelText = NSLOCTEXT("Modio", "UsedSpace", "Used: ");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText FreeSpaceLabelText = NSLOCTEXT("Modio", "FreeSpace", "Free: ");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TotalSpaceLabelText = NSLOCTEXT("Modio", "TotalSpace", "Total disk space: ");
};
