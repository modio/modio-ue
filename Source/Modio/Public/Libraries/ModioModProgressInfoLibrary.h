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
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioModProgressInfo.h"
#include "Types/ModioUnsigned64.h"

#include "ModioModProgressInfoLibrary.generated.h"

/**
 * @docpublic
 * @brief Utility functions for accessing and managing mod operation progress
 */
UCLASS()
class MODIO_API UModioModProgressInfoLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Get the current state of the mod operation
	 * @param Info The progress struct to query
	 * @return An EModioModProgressState indicating the current state of the mod operation
	 */
	UFUNCTION(BlueprintPure, Category = "ModProgressInfo")
	static EModioModProgressState GetCurrentState(const FModioModProgressInfo& Info)
	{
		return Info.GetCurrentState();
	}

	/**
	 * @docpublic
	 * @brief Retrieve the progress value for a specified state
	 * If the state has completed, `CurrentProgress` will equal `TotalProgress`. For example, if a mod 
	 * is currently in the Extracting state, querying progress for the Downloading state will return a value 
	 * equal to the total download size because the download is complete.
	 * @param Info The progress struct to query
	 * @param State The state to query progress information for
	 * @return An FModioUnsigned64 representing the current progress in bytes
	 */
	UFUNCTION(BlueprintPure, Category = "ModProgressInfo")
	static FModioUnsigned64 GetCurrentProgress(const FModioModProgressInfo& Info, EModioModProgressState State)
	{
		return Info.GetCurrentProgress(State);
	}

	/**
	 * @docpublic
	 * @brief Retrieve the total amount of progress required for a specified state
	 * @param Info The progress struct to query
	 * @param State The state to query total progress for
	 * @return An FModioUnsigned64 representing the total progress in bytes
	 */
	UFUNCTION(BlueprintPure, Category = "ModProgressInfo")
	static FModioUnsigned64 GetTotalProgress(const FModioModProgressInfo& Info, EModioModProgressState State)
	{
		return Info.GetTotalProgress(State);
	}
};