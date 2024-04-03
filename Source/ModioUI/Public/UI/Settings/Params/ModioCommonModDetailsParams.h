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
#include "ModioCommonModDetailsParams.generated.h"

/**
 * Project Settings customization for ModioCommonModDetailsView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonModDetailsParamsSettings
{
	GENERATED_BODY()

	FModioCommonModDetailsParamsSettings()
	{
		SubscribeInputAction.RowName = "Tertiary";
		SubscribeInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		CancelInputAction.RowName = "Tertiary";
		CancelInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		RateUpInputAction.RowName = "Tertiary";
		RateUpInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		RateDownInputAction.RowName = "Secondary";
		RateDownInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		OpenReportInputAction.RowName = "RightTabSecondary";
		OpenReportInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		SwitchEnabledInputAction.RowName = "LeftTabSecondary";
		SwitchEnabledInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SubscribeLabel = NSLOCTEXT("Modio", "Subscribe", "Subscribe");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText UnsubscribeLabel = NSLOCTEXT("Modio", "Unsubscribe", "Unsubscribe");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CancelLabel = NSLOCTEXT("Modio", "Cancel", "Cancel");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText RateUpLabel = NSLOCTEXT("Modio", "RateUp", "Rate Up");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText RateDownLabel = NSLOCTEXT("Modio", "RateDown", "Rate Down");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ReportLabel = NSLOCTEXT("Modio", "Report", "Report");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText FileSizeLabel = NSLOCTEXT("Modio", "FileSize", "File Size:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText LastUpdatedLabel = NSLOCTEXT("Modio", "LastUpdated", "Last Updated:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ReleaseDateLabel = NSLOCTEXT("Modio", "ReleaseDate", "Release Date:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SubscribersLabel = NSLOCTEXT("Modio", "Subscribers", "Subscribers:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CreatedByLabel = NSLOCTEXT("Modio", "CreatedBy", "Created By:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TagsLabel = NSLOCTEXT("Modio", "Tags", "Tags:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ModDescriptionLabel = NSLOCTEXT("Modio", "ModDescription", "Description");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SimilarModsLabel = NSLOCTEXT("Modio", "SimilarMods", "Similar Mods");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DownloadingLabel = NSLOCTEXT("Modio", "Downloading", "Downloading");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ExtractingLabel = NSLOCTEXT("Modio", "Extracting", "Extracting");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText EnableLabel = NSLOCTEXT("Modio", "Enable", "Enable");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DisableLabel = NSLOCTEXT("Modio", "Disable", "Disable");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle SubscribeInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle CancelInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle RateUpInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle RateDownInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle OpenReportInputAction;
	
	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle SwitchEnabledInputAction;
};
