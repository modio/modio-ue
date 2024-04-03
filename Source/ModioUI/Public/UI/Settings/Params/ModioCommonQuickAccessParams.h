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
#include "ModioCommonQuickAccessParams.generated.h"

/**
 * Project Settings customization for ModioCommonQuickAccessTabView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonQuickAccessParamsSettings
{
	GENERATED_BODY()

public:
	FModioCommonQuickAccessParamsSettings()
	{
		MyCollectionInputAction.RowName = "SpecialRight";
		MyCollectionInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		MainGameMenuInputAction.RowName = "Tertiary";
		MainGameMenuInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		AuthInputAction.RowName = "Secondary";
		AuthInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText MainGameMenuButtonLabel = NSLOCTEXT("Modio", "MainMenu", "Main Menu");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText MyCollectionButtonLabel = NSLOCTEXT("Modio", "MyCollection", "My Collection");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText LogInButtonLabel = NSLOCTEXT("Modio", "LogInButtonLabel", "Log In");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText LogOutButtonLabel = NSLOCTEXT("Modio", "LogOutButtonLabel", "Log Out");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle MainGameMenuInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle AuthInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle MyCollectionInputAction;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Bool")
	bool bShowMainGameMenu = true;
};
