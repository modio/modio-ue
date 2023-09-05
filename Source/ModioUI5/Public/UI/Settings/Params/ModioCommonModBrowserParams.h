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
#include "Types/ModioFilterParams.h"
#include "UI/Settings/ModioCommonDefines.h"
#include "Engine/DataTable.h"
#include "ModioCommonModBrowserParams.generated.h"

/**
 * Struct that stores mod attributes like category name or tags
 */
USTRUCT(BlueprintType)
struct MODIOUI5_API FModioCommonFeaturedCategoryParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Base")
	FText CategoryName;

	UPROPERTY(EditDefaultsOnly, Category = "Base")
	TArray<FString> Tags;

	UPROPERTY(EditDefaultsOnly, Category = "Base")
	TArray<FString> ExcludedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Base")
	EModioSortDirection Direction;

	UPROPERTY(EditDefaultsOnly, Category = "Base")
	EModioSortFieldType SortField;

	UPROPERTY(EditDefaultsOnly, Category = "Base")
	int64 Count = 20;

	FModioFilterParams ToFilterParams() const
	{
		return FModioFilterParams()
		       .WithTags(Tags)
		       .WithoutTags(ExcludedTags)
		       .SortBy(SortField, Direction)
		       .IndexedResults(0, Count);
	}
};

/**
 * Project Settings customization for ModioCommonFeaturedPrimaryView
 */
UCLASS(Config = "ModioCommonFeaturedPrimaryParams", DefaultConfig, meta = (DisplayName = "Featured Primary Params"))
class MODIOUI5_API UModioCommonFeaturedPrimaryParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly, Category = "Featured|Primary")
	FModioCommonFeaturedCategoryParams PrimaryCategoryParam
	{
		[] {
			FModioCommonFeaturedCategoryParams PrimaryParam;
			PrimaryParam.Direction = EModioSortDirection::Ascending;
			PrimaryParam.SortField = EModioSortFieldType::DownloadsToday;
			PrimaryParam.Count = 20;
			return PrimaryParam;
		}()
	};

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};

/**
 * Project Settings customization for ModioCommonFeaturedAdditionalView
 */
UCLASS(Config = "ModioCommonFeaturedAdditionalParams", DefaultConfig, meta = (DisplayName = "Featured Additional Params"))
class MODIOUI5_API UModioCommonFeaturedAdditionalParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UModioCommonFeaturedAdditionalParamsSettings()
	{
		PreviousTabInputAction.RowName = "LeftTabSecondary";
		PreviousTabInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		NextTabInputAction.RowName = "RightTabSecondary";
		NextTabInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}
	
	UPROPERTY(Config, EditDefaultsOnly, Category = "Featured|Additional")
	TArray<FModioCommonFeaturedCategoryParams> AdditionalCategoryParams
	{
		[] {
			FModioCommonFeaturedCategoryParams MostRecent;
			MostRecent.CategoryName = NSLOCTEXT("Modio", "MostPopular", "Most Popular");
			MostRecent.Direction = EModioSortDirection::Descending;
			MostRecent.SortField = EModioSortFieldType::DownloadsTotal;
			MostRecent.Count = 20;
			return MostRecent;
		}(),
		[] {
			FModioCommonFeaturedCategoryParams MostRecommended;
			MostRecommended.CategoryName = NSLOCTEXT("Modio", "HighestRated", "Highest Rated");
			MostRecommended.Direction = EModioSortDirection::Descending;
			MostRecommended.SortField = EModioSortFieldType::Rating;
			MostRecommended.Count = 20;
			return MostRecommended;
		}(),
		[] {
			FModioCommonFeaturedCategoryParams MostPopular;
			MostPopular.CategoryName = NSLOCTEXT("Modio", "RecentlyAdded", "Recently Added");
			MostPopular.Direction = EModioSortDirection::Descending;
			MostPopular.SortField = EModioSortFieldType::DateMarkedLive;
			MostPopular.Count = 20;
			return MostPopular;
		}()
	};

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle PreviousTabInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle NextTabInputAction;

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};

/**
 * Project Settings customization for ModioCommonModBrowser
 */
UCLASS(Config = "ModioCommonModBrowserParams", DefaultConfig, meta = (DisplayName = "Mod Browser Params"))
class MODIOUI5_API UModioCommonModBrowserParamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UModioCommonModBrowserParamsSettings()
	{
		PreviousTabInputAction.RowName = "LeftTab";
		PreviousTabInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());

		NextTabInputAction.RowName = "RightTab";
		NextTabInputAction.DataTable = Cast<UDataTable>(FSoftObjectPath(ModioInputActionDataTablePath).TryLoad());
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Mod Browser")
	FText FeaturedViewTabText = NSLOCTEXT("Modio", "FeaturedViewTabText", "Featured");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Mod Browser")
	FText CollectionViewTabText = NSLOCTEXT("Modio", "CollectionViewTabText", "Collection");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Mod Browser")
	FText SearchResultsViewTabText = NSLOCTEXT("Modio", "SearchResultsViewTabText", "Search");

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle PreviousTabInputAction;

	UPROPERTY(Config, EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase), Category = "Actions")
	FDataTableRowHandle NextTabInputAction;

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return ModioCommonCategoryName; }
	// End UDeveloperSettings Interface
};
