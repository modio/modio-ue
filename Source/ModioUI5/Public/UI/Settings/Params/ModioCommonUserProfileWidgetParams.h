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
UCLASS(Config = "ModioCommonUserProfileWidgetParams", DefaultConfig, meta = (DisplayName = "User Profile Widget Params"))
class MODIOUI5_API UModioCommonUserProfileWidgetParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UModioCommonUserProfileWidgetParamsSettings()
	{
		ProfileInputAction.RowName = "Secondary";
		ProfileInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ProfileButtonLabel = NSLOCTEXT("Modio", "ProfileButtonLabel", "Profile");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle ProfileInputAction;

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};
