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
* Basic reference data about a dependency relationship to another mod
* @experimental
**/
USTRUCT(BlueprintType)
struct MODIO_API FModioModDependency
{
	GENERATED_BODY();
	
	/**
	* The ID of the mod dependency
	**/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	FModioModID ModID;

	/**
	* The name of the mod dependency
	**/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	FString ModName;
};


/**
* Strong type struct to wrap multiple ModDependency indexed by a paged result
* @experimental
**/
USTRUCT(BlueprintType)
struct MODIO_API FModioModDependencyList
{
	GENERATED_BODY();

	/**
	* Stored property for a paged result, which provides context on the dependency list
	**/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependencyList")
	FModioPagedResult PagedResult;

	/**
	* Stored property for the dependency list
	**/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependencyList")
	TArray<FModioModDependency> InternalList;
};

/**
* Strong type struct to wrap a ModDependencyList data as an optional value
**/
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModDependencyList
{
	GENERATED_BODY()

    /**
    * Default constructor without parameters
    **/
	FModioOptionalModDependencyList() = default;
	
    /**
    * Convenience constructor that has a list of mod dependencies
    * @param ModDependencies An optional value that contains mod dependencies 
    **/
	FModioOptionalModDependencyList(TOptional<FModioModDependencyList>&& ModDependencies);
	
    /**
    * Stored optional ModioModDependencyList
    **/
	TOptional<FModioModDependencyList> Internal;
};
