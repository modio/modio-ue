/* 
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
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
#include "Types/ModioModCollection.h"
#include "ModioModCollectionInfoListLibrary.generated.h"

/**
 * @docpublic
 * @brief Utility functions for accessing and managing lists of mod information
 */
UCLASS()
class MODIO_API UModioModCollectionInfoListLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Retrieve the list of mod collections from the mod collection info list
	 * @param ModCollectionInfoList The mod collection info list to retrieve the mod collections from
	 * @return An array of FModioModCollectionInfo objects representing the mod collections in the list
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const TArray<FModioModCollectionInfo>& GetCollections(
		const FModioModCollectionInfoList& ModCollectionInfoList);

	/**
	 * @docpublic
	 * @brief Retrieve paged result information for a mod collection info list
	 * @param ModCollectionInfoList The mod collection info list to retrieve paged result information from
	 * @return An FModioPagedResult containing metadata about the paginated data
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const FModioPagedResult& GetPagedResult(const FModioModCollectionInfoList& ModCollectionInfoList);
};