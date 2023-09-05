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
#include "ModioCommonFilteredModListParams.generated.h"

/**
 * Project Settings customization for ModioCommonFilteredModListView
 */
UCLASS(Config = "ModioCommonFilteredModListParams", DefaultConfig, meta = (DisplayName = "Filtered Mod List Params"))
class MODIOUI5_API UModioCommonFilteredModListParams : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UModioCommonFilteredModListParams()
	{
		PreviousPageInputAction.RowName = "LeftTabSecondary";
		PreviousPageInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		NextPageInputAction.RowName = "RightTabSecondary";
		NextPageInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText PreviousPageLabel = NSLOCTEXT("Modio", "PreviousPageButtonText", "Previous Page");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText NextPageLabel = NSLOCTEXT("Modio", "NextPageButtonText", "Next Page");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText CurrentPageTextFormat = NSLOCTEXT("Modio", "CurrentPageTextFormat", "Page {0}");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TotalPagesTextFormat = NSLOCTEXT("Modio", "TotalPagesTextFormat", " of {0}");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText TotalModsTextFormat = NSLOCTEXT("Modio", "CurrentPageTextFormat", "Total of {0} mods found");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle PreviousPageInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle NextPageInputAction;

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};
