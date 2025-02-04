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
#include "Types/ModioModInfoList.h"

#include "ModioModInfoListLibrary.generated.h"


/**
 * @docpublic
 * @brief Utility functions for accessing and managing lists of mod information
 */
UCLASS()
class MODIO_API UModioModInfoListLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Retrieve the list of mods from the mod info list
	 * @param ModInfoList The mod info list to retrieve the mods from
	 * @return An array of FModioModInfo objects representing the mods in the list
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const TArray<FModioModInfo>& GetMods(const FModioModInfoList& ModInfoList);

	/**
	 * @docpublic
	 * @brief Retrieve paged result information for a mod info list
	 * @param ModInfoList The mod info list to retrieve paged result information from
	 * @return An FModioPagedResult containing metadata about the paginated data
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const FModioPagedResult& GetPagedResult(const FModioModInfoList& ModInfoList);
};