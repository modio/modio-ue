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
#include "Types/ModioModDependencyList.h"

#include "ModioModDependenciesLibrary.generated.h"


UCLASS()
class MODIO_API UModioModDependenciesLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * Get the tags in a mod tag options list
	 * @param DependencyList 
	 * @return
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const TArray<FModioModDependency>& GetDependencies(const FModioModDependencyList& DependencyList);

	/**
	 * Get the paged result that contains information of the data returned
	 * @param DependencyList
	 * @return
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const FModioPagedResult& GetPagedResult(const FModioModDependencyList& DependencyList);

	/**
	 * Get the total size of all the dependency files in bytes.
	 * @param DependencyList
	 * @return
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const int64& GetTotalFilesize(const FModioModDependencyList& DependencyList);

	/**
	 * Get the total Size of the uncompressed dependency files in bytes
	 * @param DependencyList
	 * @return
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const int64& GetTotalFilesizeUncompressed(const FModioModDependencyList& DependencyList);
};
