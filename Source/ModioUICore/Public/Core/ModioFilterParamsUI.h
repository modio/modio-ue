/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Types/ModioFilterParams.h"
#include "UObject/NoExportTypes.h"

#include "ModioFilterParamsUI.generated.h"

/**
* Wrapper around FModioFilterParams because UMG widgets expect UObjects for data sources or list items
**/
UCLASS(BlueprintType)
class MODIOUICORE_API UModioFilterParamsUI : public UObject
{
	GENERATED_BODY()

public:
	/**
	* Stored property of the filter params in this class
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioFilterParamsUI")
	FModioFilterParams Underlying;
};

UENUM(BlueprintType)
enum class EModioEnabledFilterType : uint8
{
	None,
	Enabled,
	Disabled,
};

UENUM(BlueprintType)
enum class EModioInstalledFilterType : uint8
{
	None,
	CurrentUser,
	AnotherUser,
};

UENUM(BlueprintType)
enum class EModioManualSortType : uint8
{
	AToZ,
	ZToA,
	SizeOnDisk,
	RecentlyUpdated,
};

/**
 * Struct that stores mod attributes like category name or tags
 */
USTRUCT(BlueprintType)
struct MODIOUICORE_API FModioModCategoryParams
{
	GENERATED_BODY()

	FModioModCategoryParams() = default;

	FModioModCategoryParams(const FModioFilterParams& FilterParams)
	{
		Tags = FilterParams.Tags;
		ExcludedTags = FilterParams.ExcludedTags;
		Direction = FilterParams.Direction;
		SortField = FilterParams.SortField;
		Count = FilterParams.Count;
		SearchKeywords = FilterParams.SearchKeywords.Num() > 0 ? FilterParams.SearchKeywords[0] : FString();
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base")
	FText CategoryName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base")
	TArray<FString> Tags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base")
	TArray<FString> ExcludedTags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base")
	EModioSortDirection Direction = EModioSortDirection::Ascending;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base")
	EModioSortFieldType SortField = EModioSortFieldType::ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base")
	int64 Count = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base")
	FString SearchKeywords;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom")
	EModioInstalledFilterType InstalledField = EModioInstalledFilterType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom")
	EModioEnabledFilterType EnabledFilter = EModioEnabledFilterType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom")
	EModioManualSortType ManualSortField = EModioManualSortType::AToZ;

	bool operator==(const FModioModCategoryParams& Other) const
	{
		return CategoryName.EqualTo(Other.CategoryName) &&
			   Tags == Other.Tags &&
			   ExcludedTags == Other.ExcludedTags &&
			   Direction == Other.Direction &&
			   SortField == Other.SortField &&
			   Count == Other.Count &&
			   SearchKeywords == Other.SearchKeywords &&
			   InstalledField == Other.InstalledField &&
			   EnabledFilter == Other.EnabledFilter &&
			   ManualSortField == Other.ManualSortField;
	}

	bool operator!=(const FModioModCategoryParams& Other) const
	{
		return !(*this == Other);
	}

	FModioFilterParams ToFilterParams() const
	{
		return FModioFilterParams()
			   .WithTags(Tags)
			   .WithoutTags(ExcludedTags)
			   .SortBy(SortField, Direction)
			   .IndexedResults(0, Count)
			   .NameContains(SearchKeywords.IsEmpty() ? TArray<FString>() : TArray<FString>{SearchKeywords});
	}
};

UCLASS(BlueprintType)
class MODIOUICORE_API UModioModCategoryParamsUI : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Stored property of the filter params in this class
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioModCategoryParamsUI")
	FModioModCategoryParams Underlying;
};