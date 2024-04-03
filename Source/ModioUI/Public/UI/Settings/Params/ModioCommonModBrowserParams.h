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
#include "Core/ModioFilterParamsUI.h"
#include "Engine/DeveloperSettings.h"
#include "Types/ModioFilterParams.h"
#include "UI/Settings/ModioCommonDefines.h"
#include "Engine/DataTable.h"
#include "ModioCommonModBrowserParams.generated.h"

/**
 * Project Settings customization for ModioCommonCollectionView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonCollectionParamsSettings
{
	GENERATED_BODY()

	FModioCommonCollectionParamsSettings()
	{
		FilterInputAction.RowName = "Secondary";
		FilterInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		CheckForUpdatesInputAction.RowName = "Quinary";
		CheckForUpdatesInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText ErrorsLabel = NSLOCTEXT("Modio", "Errors", "Errors:");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText InstalledModsLabel = NSLOCTEXT("Modio", "InstalledModsLabel", "Installed");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText InstalledModsDescription = NSLOCTEXT("Modio", "InstalledModsDescription", "installed mods");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText DefaultFetchUpdateButtonLabel = NSLOCTEXT("Modio", "DefaultFetchUpdateButtonLabel", "Check for updates");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText SearchingFetchUpdateButtonLabel = NSLOCTEXT("Modio", "SearchingFetchUpdateButtonLabel", "Searching");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText OwnedModsLabel = NSLOCTEXT("Modio", "OwnedModsLabel", "Owned");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Text")
	FText FilterButtonLabel = NSLOCTEXT("Modio", "CollectionFilterButtonLabel", "Filter");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle CheckForUpdatesInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle FilterInputAction;
};

/**
 * Project Settings customization for ModioCommonFeaturedAdditionalView
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonFeaturedParamsSettings
{
	GENERATED_BODY()
	
	UPROPERTY(Config, EditDefaultsOnly, Category = "Featured|Additional")
	TArray<FModioModCategoryParams> CategoryParams
	{
		[] {
			FModioModCategoryParams MostPopular;
			MostPopular.CategoryName = NSLOCTEXT("Modio", "MostPopular", "Popular");
			MostPopular.Direction = EModioSortDirection::Descending;
			MostPopular.SortField = EModioSortFieldType::DownloadsTotal;
			MostPopular.Count = 20;
			return MostPopular;
		}(),
		[] {
			FModioModCategoryParams MostRecent;
			MostRecent.CategoryName = NSLOCTEXT("Modio", "RecentlyAdded", "Recent");
			MostRecent.Direction = EModioSortDirection::Descending;
			MostRecent.SortField = EModioSortFieldType::DateMarkedLive;
			MostRecent.Count = 20;
			return MostRecent;
		}()
	};
};

/**
 * Project Settings customization for ModioCommonModBrowser
 */
USTRUCT(BlueprintType, Category = "Mod.io Common UI")
struct MODIOUI_API FModioCommonModBrowserParamsSettings
{
	GENERATED_BODY()

	FModioCommonModBrowserParamsSettings()
	{
		PreviousTabInputAction.RowName = "LeftTab";
		PreviousTabInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		NextTabInputAction.RowName = "RightTab";
		NextTabInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Mod Browser")
	FText CollectionViewTabText = NSLOCTEXT("Modio", "CollectionViewTabText", "Collection");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Mod Browser")
	FText SearchResultsViewTabText = NSLOCTEXT("Modio", "SearchResultsViewTabText", "Search");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle PreviousTabInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"), Category = "Actions")
	FDataTableRowHandle NextTabInputAction;
};
