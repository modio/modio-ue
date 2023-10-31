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
#include "Engine/DataTable.h"
#include "ModioCommonModCollectionParams.generated.h"

/**
 * Project Settings customization for ModioCommonCollectionView
 */
UCLASS(Config = "ModioCommonModCollectionParams", DefaultConfig, meta = (DisplayName = "Mod Collection Params"))
class MODIOUI5_API UModioCommonModCollectionParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UModioCommonModCollectionParamsSettings()
	{
		CheckForUpdatesInputAction.RowName = "Back";
		CheckForUpdatesInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		PreviousTabInputAction.RowName = "LeftTabSecondary";
		PreviousTabInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		NextTabInputAction.RowName = "RightTabSecondary";
		NextTabInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText AllInstalledCategoryName = NSLOCTEXT("Modio", "AllInstalled", "All Installed");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SystemModsCategoryName = NSLOCTEXT("Modio", "SystemMods", "System Mods");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DownloadingModsLabel = NSLOCTEXT("Modio", "DownloadingModsLabel", "Downloading");
	
	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DownloadingModsDescription = NSLOCTEXT("Modio", "DownloadingModsDescription", "Downloading Mods:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ErrorsLabel = NSLOCTEXT("Modio", "Errors", "Errors:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText InstalledModsLabel = NSLOCTEXT("Modio", "InstalledModsLabel", "Installed");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText InstalledModsDescription = NSLOCTEXT("Modio", "InstalledModsDescription", "Installed Mods:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DefaultFetchUpdateButtonLabel = NSLOCTEXT("Modio", "DefaultFetchUpdateButtonLabel", "Check for updates");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SearchingFetchUpdateButtonLabel = NSLOCTEXT("Modio", "SearchingFetchUpdateButtonLabel", "Searching");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle PreviousTabInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle NextTabInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle CheckForUpdatesInputAction;

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};
