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
#include "ModioCommonModGalleryParams.generated.h"

/**
 * Project Settings customization for ModioCommonModGalleryView
 */
UCLASS(Config = "ModioCommonModGalleryParams", DefaultConfig, meta = (DisplayName = "Mod Gallery Params"))
class MODIOUI5_API UModioCommonModGalleryParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UModioCommonModGalleryParamsSettings()
	{
		PreviousImageInputAction.RowName = "LeftTabSecondary";
		PreviousImageInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		NextImageInputAction.RowName = "RightTabSecondary";
		NextImageInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		SubscribeInputAction.RowName = "Primary";
		SubscribeInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle PreviousImageInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle NextImageInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle SubscribeInputAction;

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};
