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
#include "Misc/Optional.h"
#include "Types/ModioCommonTypes.h"

#include "ModioModChangeMap.generated.h"

/**
 * @docpublic
 * @brief Strong type struct to wrap mod change information
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModChangeMap
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief A map representing mod change types for each mod ID
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModChangeMap")
	TMap<FModioModID, EModioModChangeType> Changes;
};

/**
 * @docpublic
 * @brief Strong type struct to wrap a ModChangeMap as an optional value
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModChangeMap
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioOptionalModChangeMap() = default;

	/**
	 * @docpublic
	 * @brief Convenience constructor that has a list of mod changes
	 * @param ModDependencies An optional value that contains mod change information
	 */
	FModioOptionalModChangeMap(TOptional<FModioModChangeMap>&& ModDependencies) : Internal(MoveTemp(ModDependencies)) {}

	/**
	 * @docpublic
	 * @brief Stored optional ModioModChangeMap
	 */
	TOptional<FModioModChangeMap> Internal;
};