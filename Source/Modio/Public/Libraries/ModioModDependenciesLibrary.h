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

/**
 * @docpublic
 * @brief Utility functions for accessing and managing mod dependencies
 */
UCLASS()
class MODIO_API UModioModDependenciesLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Retrieve the list of mod dependencies from the dependency list
	 * @param DependencyList The dependency list to retrieve mod dependencies from
	 * @return An array of FModioModDependency representing the dependencies of a mod
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const TArray<FModioModDependency>& GetDependencies(const FModioModDependencyList& DependencyList);

	/**
	 * @docpublic
	 * @brief Retrieve paged result information for a dependency list
	 * @param DependencyList The dependency list to retrieve paged result information from
	 * @return An FModioPagedResult containing metadata about the paginated data
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static const FModioPagedResult& GetPagedResult(const FModioModDependencyList& DependencyList);

	/**
	 * @docpublic
	 * @brief Get the total size of all dependency files in bytes
	 * @param DependencyList The dependency list to calculate the total file size for
	 * @return An int64 representing the total size of all dependency files in bytes
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static FModioUnsigned64 GetTotalFilesize(const FModioModDependencyList& DependencyList);

	/**
	 * @docpublic
	 * @brief Get the total size of all uncompressed dependency files in bytes
	 * @param DependencyList The dependency list to calculate the total uncompressed file size for
	 * @return An int64 representing the total size of all uncompressed dependency files in bytes
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static FModioUnsigned64 GetTotalFilesizeUncompressed(const FModioModDependencyList& DependencyList);
};
