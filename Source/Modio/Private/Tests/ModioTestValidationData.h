// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Types/ModioCommonTypes.h"
#include "UObject/NoExportTypes.h"

#include "ModioTestValidationData.generated.h"

USTRUCT()
struct FModioTestFilterParamData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Testing")
	FModioModID First;

	UPROPERTY(EditDefaultsOnly, Category = "Testing")
	FModioModID Last;
};

/** @brief Config class containing unit test verification data
 *
 */
UCLASS(Config = ModioTest, defaultconfig)
class UModioTestValidationData : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, config, Category = "Testing")
	FModioTestFilterParamData SortDateMarkedLive;

	UPROPERTY(EditDefaultsOnly, config, Category = "Testing")
	FModioTestFilterParamData SortDateUpdated;

	UPROPERTY(EditDefaultsOnly, config, Category = "Testing")
	FModioTestFilterParamData SortDownloadsToday;

	UPROPERTY(EditDefaultsOnly, config, Category = "Testing")
	FModioTestFilterParamData SortID;

	UPROPERTY(EditDefaultsOnly, config, Category = "Testing")
	FModioTestFilterParamData SortRating;

	UPROPERTY(EditDefaultsOnly, config, Category = "Testing")
	FModioTestFilterParamData SortSubCount;

	UPROPERTY(EditDefaultsOnly, config, Category = "Testing")
	FModioModID NameSubstringSearchFirstID;
};
