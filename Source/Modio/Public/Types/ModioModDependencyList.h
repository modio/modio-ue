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
#include "Types/ModioCommonTypes.h"
#include "Types/ModioPagedResult.h"
#include "Containers/UnrealString.h"
#include "Misc/Optional.h"

#include "ModioModDependencyList.generated.h"

/**
 * @brief Minimal data about a mod which is a dependency of another mod
 * @experimental
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModDependency
{
	GENERATED_BODY();
	
	/**
	 * @brief The ID of the dependency
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	FModioModID ModID;

	/**
	 * @brief The name of the dependency
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	FString ModName;
};


/**
 * @brief Paged list of mod dependencies
 * @experimental
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModDependencyList
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependencyList")
	FModioPagedResult PagedResult;

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependencyList")
	TArray<FModioModDependency> InternalList;
};



USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModDependencyList
{
	GENERATED_BODY()

	FModioOptionalModDependencyList() = default;
	FModioOptionalModDependencyList(TOptional<FModioModDependencyList>&& ModDependencies);

	TOptional<FModioModDependencyList> Internal;
};
