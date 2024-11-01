/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

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
