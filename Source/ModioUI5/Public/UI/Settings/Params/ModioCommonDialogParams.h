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
#include "ModioCommonDialogParams.generated.h"

/**
 * Project Settings customization for ModioCommonDialogView
 */
UCLASS(Config = "ModioCommonDialogParams", DefaultConfig, meta = (DisplayName = "Dialog Params"))
class MODIOUI5_API UModioCommonDialogParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UModioCommonDialogParamsSettings()
	{
		SubmitInputAction.RowName = "Primary";
		SubmitInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle SubmitInputAction;

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};

/**
 * Project Settings customization for ModioCommonDialogMessageView
 */
UCLASS(Config = "ModioCommonDialogMessageParams", DefaultConfig, meta = (DisplayName = "Dialog Message Params"))
class MODIOUI5_API UModioCommonDialogMessageParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SubmitButtonText = NSLOCTEXT("Modio", "SubmitButtonText", "OK");

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};