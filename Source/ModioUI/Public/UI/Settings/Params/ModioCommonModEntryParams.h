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
#include "Engine/DataTable.h"
#include "ModioCommonModEntryParams.generated.h"

/**
 * Project Settings customization for ModioCommonGenericModEntry
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonModEntryParamsSettings
{
	GENERATED_BODY()

	FModioCommonModEntryParamsSettings()
	{
		OpenModDetailsInputAction.RowName = "Primary";
		OpenModDetailsInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		SubscribeInputAction.RowName = "Tertiary";
		SubscribeInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		CancelInputAction.RowName = "Tertiary";
		CancelInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		SwitchEnabledInputAction.RowName = "LeftTabSecondary";
		SwitchEnabledInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		ForceUninstallInputAction.RowName = "LeftTabSecondary";
		ForceUninstallInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		PrioritizeTransferInputAction.RowName = "LeftTabSecondary";
		PrioritizeTransferInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SubscribeLabel = NSLOCTEXT("Modio", "Subscribe", "Subscribe");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText UnsubscribeLabel = NSLOCTEXT("Modio", "Unsubscribe", "Unsubscribe");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelLabel = NSLOCTEXT("Modio", "Cancel", "Cancel");
	
	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText EnableLabel = NSLOCTEXT("Modio", "Enable", "Enable");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DisableLabel = NSLOCTEXT("Modio", "Disable", "Disable");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ForceUninstallLabel = NSLOCTEXT("Modio", "ForceUninstall", "Force Uninstall");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText OpenModDetailsLabel = NSLOCTEXT("Modio", "OpenModDetails", "Mod Details");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText PrioritizeTransferLabel = NSLOCTEXT("Modio", "PrioritizeTransfer", "Prioritize Transfer");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText OpenReportLabel = NSLOCTEXT("Modio", "OpenReport", "Report");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DownloadingLabel = NSLOCTEXT("Modio", "Downloading", "Downloading - ");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ExtractingLabel = NSLOCTEXT("Modio", "Extracting", "Extracting - ");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText QueuedLabel = NSLOCTEXT("Modio", "Queued", "Queued");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SpeedLabel = NSLOCTEXT("Modio", "Speed", "Speed - ");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SizeLabel = NSLOCTEXT("Modio", "Size", "Size - ");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ErrorLabel = NSLOCTEXT("Modio", "Error", "Error - ");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText InstalledByOthersLabel = NSLOCTEXT("Modio", "InstalledByOthers", "Installed by others");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Delays", meta = (UIMin = 0.0, UIMax = 2.0))
	float DeselectionDelay = 0.5f;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle SubscribeInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle CancelInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle SwitchEnabledInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle OpenModDetailsInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle ForceUninstallInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle PrioritizeTransferInputAction;
};
