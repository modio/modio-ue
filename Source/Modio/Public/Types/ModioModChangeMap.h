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

USTRUCT(BlueprintType)
struct MODIO_API FModioModChangeMap
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModChangeMap")
	TMap<FModioModID, EModioModChangeType> Changes;
};

/**
 * Strong type struct to wrap a ModDependencyList data as an optional value
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModChangeMap
{
	GENERATED_BODY()

	/**
	 * Default constructor without parameters
	 **/
	FModioOptionalModChangeMap() = default;

	/**
	 * Convenience constructor that has a list of mod dependencies
	 * @param ModDependencies An optional value that contains mod dependencies
	 **/
	FModioOptionalModChangeMap(TOptional<FModioModChangeMap>&& ModDependencies) : Internal(MoveTemp(Internal)) {}

	/**
	 * Stored optional ModioModDependencyList
	 **/
	TOptional<FModioModChangeMap> Internal;
};
