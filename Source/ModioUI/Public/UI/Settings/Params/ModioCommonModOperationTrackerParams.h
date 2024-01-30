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
#include "ModioCommonModOperationTrackerParams.generated.h"

/**
 * Project Settings customization for ModioCommonModOperationTrackerUserWidget
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonModOperationTrackerParamsSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText OverallOperationPercentageLabelText = NSLOCTEXT("Modio", "OverallOperationPercentage", "Overall: ");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText QueuedOperationNumberLabelText = NSLOCTEXT("Modio", "QueuedOperationNumber", "Queued: ");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SpeedLabelText = NSLOCTEXT("Modio", "Speed", "Speed");
};
