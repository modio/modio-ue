﻿/*
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
#include "ModioCommonQuickAccessParams.generated.h"

/**
 * Project Settings customization for ModioCommonQuickAccessTabView
 */
UCLASS(Config = "ModioCommonModCollectionParams", DefaultConfig, meta = (DisplayName = "Quick Access Params"))
class MODIOUI5_API UModioCommonQuickAccessParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UModioCommonQuickAccessParamsSettings()
	{
		CloseInputAction.RowName = "Primary";
		CloseInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		MyProfileInputAction.RowName = "Tertiary";
		MyProfileInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		AuthInputAction.RowName = "Secondary";
		AuthInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		MyCollectionInputAction.RowName = "RightTabSecondary";
		MyCollectionInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CurrentDownloadsLabel = NSLOCTEXT("Modio", "CurrentDownloads", "Current Downloads");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText InQueueLabel = NSLOCTEXT("Modio", "InQueue", "In Queue");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CompletedLabel = NSLOCTEXT("Modio", "RecentDownloads", "Recent Downloads");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CloseButtonLabel = NSLOCTEXT("Modio", "CloseButtonLabel", "Close");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText MyProfileButtonLabel = NSLOCTEXT("Modio", "MyProfile", "My Profile");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText MyCollectionButtonLabel = NSLOCTEXT("Modio", "MyCollection", "My Collection");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText LogInButtonLabel = NSLOCTEXT("Modio", "LogInButtonLabel", "Log In");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText LogOutButtonLabel = NSLOCTEXT("Modio", "LogOutButtonLabel", "Log Out");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText InstalledModsLabel = NSLOCTEXT("Modio", "InstalledModsLabel", "mods installed");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle CloseInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle MyProfileInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle AuthInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle MyCollectionInputAction;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Bool")
	bool ShowProfileButton = false;

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};