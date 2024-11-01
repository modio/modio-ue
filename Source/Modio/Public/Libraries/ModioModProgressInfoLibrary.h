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
 *
 */
UCLASS()
class MODIO_API UModioModProgressInfoLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief Returns a EModioModProgressState indicating which state the mod operation is in
	 * @param Info the progress struct to query
	 */
	UFUNCTION(BlueprintPure, Category = "ModProgressInfo")
	static EModioModProgressState GetCurrentState(const FModioModProgressInfo& Info)
	{
		return Info.GetCurrentState();
	}
	/**
	 * @brief Retrieves the progress value for the specified state. `CurrentProgress == TotalProgress` for states
	 * which have completed, for example if a mod is currently Extracting, then passing in Downloading would give
	 * you a value equal to the total download size because the download has completed
	 * @param Info the progress struct to query
	 * @param State which state to query progress information for
	 * @return FModioUnsigned64 containing current progress in bytes
	 */
	UFUNCTION(BlueprintPure, Category = "ModProgressInfo")
	static FModioUnsigned64 GetCurrentProgress(const FModioModProgressInfo& Info, EModioModProgressState State)
	{
		return Info.GetCurrentProgress(State);
	}

	/**
	 * @brief Retrieves the total amount of progress required for the specified state.
	 * @param Info the progress struct to query
	 * @param State which state to query total progress for
	 * @return Modio::FileSize for total progress in bytes
	 */
	UFUNCTION(BlueprintPure, Category = "ModProgressInfo")
	static FModioUnsigned64 GetTotalProgress(const FModioModProgressInfo& Info, EModioModProgressState State)
	{
		return Info.GetTotalProgress(State);
	}
};
