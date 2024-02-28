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
#include "UObject/Object.h"
#include "UI/Settings/ModioCommonDefines.h"
#include "ModioCommonErrorWithRetryParams.generated.h"

/**
 * Project Settings customization for ModioCommonErrorWithRetryWidget
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonErrorWithRetryParamsSettings
{
	GENERATED_BODY()

	FModioCommonErrorWithRetryParamsSettings()
	{
		RetryInputAction.RowName = "Primary";
		RetryInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText RetryButtonTextLabel = NSLOCTEXT("Modio", "RetryButtonText", " Retry");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle RetryInputAction;
};
