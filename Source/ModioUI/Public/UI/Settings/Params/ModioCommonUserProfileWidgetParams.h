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
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"
#include "UI/Settings/ModioCommonDefines.h"
#include "ModioCommonUserProfileWidgetParams.generated.h"

/**
 * Project Settings customization for ModioCommonUserProfileWidget
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonUserProfileWidgetParamsSettings
{
	GENERATED_BODY()

public:
	FModioCommonUserProfileWidgetParamsSettings()
	{
		ProfileInputAction.RowName = "Quaternary";
		ProfileInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ProfileButtonLabel = NSLOCTEXT("Modio", "ProfileButtonLabel", "Profile");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle ProfileInputAction;
};
