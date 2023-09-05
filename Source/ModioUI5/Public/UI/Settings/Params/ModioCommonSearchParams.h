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
UCLASS(Config = "ModioCommonSearchParams", DefaultConfig, meta = (DisplayName = "Search Params"))
class MODIOUI5_API UModioCommonSearchParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UModioCommonSearchParamsSettings()
	{
		CloseInputAction.RowName = "Back";
		CloseInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		SearchInputAction.RowName = "Secondary";
		SearchInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		ClearAllInputAction.RowName = "Tertiary";
		ClearAllInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		EditSearchInputAction.RowName = "Secondary";
		EditSearchInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		PreviousTabInputAction.RowName = "LeftTab";
		PreviousTabInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		NextTabInputAction.RowName = "RightTab";
		NextTabInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SearchButtonLabel = NSLOCTEXT("Modio", "SearchButtonLabel", "Search");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ClearAllButtonLabel = NSLOCTEXT("Modio", "ClearAllButtonLabel", "Clear All");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CloseButtonLabel = NSLOCTEXT("Modio", "CloseButtonLabel", "Close");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SortingTabLabel = NSLOCTEXT("Modio", "SortingLabel", "Sorting");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText FilteringTabLabel = NSLOCTEXT("Modio", "FilteringLabel", "Filtering");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText KeywordsLabel = NSLOCTEXT("Modio", "KeywordsLabelText", "Keyword(s) & filters:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CategoriesLabel = NSLOCTEXT("Modio", "CategoriesLabelText", "Categories:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText EditSearchButtonLabel = NSLOCTEXT("Modio", "EditSearchButtonLabel", "Edit Search");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ShowingAllModsLabel = NSLOCTEXT("Modio", "ShowingAllModsLabel", "Showing all mods");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle CloseInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle SearchInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle ClearAllInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle EditSearchInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle PreviousTabInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle NextTabInputAction;

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};
