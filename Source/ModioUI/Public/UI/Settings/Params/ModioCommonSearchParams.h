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
#include "ModioCommonSearchParams.generated.h"

/**
 * Project Settings customization for ModioCommonSearchTabView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonSearchParamsSettings
{
	GENERATED_BODY()

	FModioCommonSearchParamsSettings()
	{
		CloseInputAction.RowName = "Back";
		CloseInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		SearchInputAction.RowName = "Secondary";
		SearchInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		ResetInputAction.RowName = "Tertiary";
		ResetInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		EditSearchInputAction.RowName = "Secondary";
		EditSearchInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SearchTabTitle = NSLOCTEXT("Modio", "SearchTabTitle", "Filter");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SearchButtonLabel = NSLOCTEXT("Modio", "SearchButton", "Search");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ResetButtonLabel = NSLOCTEXT("Modio", "ResetButton", "Reset");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CloseButtonLabel = NSLOCTEXT("Modio", "CloseButton", "Close");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText KeywordsLabel = NSLOCTEXT("Modio", "Keywords", "Keyword(s) & filters:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CategoriesLabel = NSLOCTEXT("Modio", "Categories", "Categories:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText EditSearchButtonLabel = NSLOCTEXT("Modio", "EditSearchButton", "Filter");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ShowingAllModsLabel = NSLOCTEXT("Modio", "ShowingAllMods", "");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle CloseInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle SearchInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle ResetInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle EditSearchInputAction;
};
