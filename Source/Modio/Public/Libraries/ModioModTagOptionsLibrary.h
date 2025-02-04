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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioModTagOptions.h"

#include "ModioModTagOptionsLibrary.generated.h"

/**
 * @docpublic
 * @brief Utility functions for accessing and managing mod tag options
 */
UCLASS()
class MODIO_API UModioModTagOptionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Retrieve the tags from a mod tag options list
	 * @param ModTags The mod tag options list to retrieve tags from
	 * @return An array of FModioModTagInfo representing the tags available for a mod
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const TArray<FModioModTagInfo>& GetTags(const FModioModTagOptions& ModTags);

	/**
	 * @docpublic
	 * @brief Retrieve paged result information for a mod tag options list
	 * @param ModTags The mod tag options list to retrieve paged result information from
	 * @return An FModioPagedResult containing metadata about the paginated data
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const FModioPagedResult& GetPagedResult(const FModioModTagOptions& ModTags);
};