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
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "Misc/Optional.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioFileMetadata.h"
#include "Types/ModioLogo.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioPagedResult.h"

#include "ModioModDependencyList.generated.h"

typedef TMap<FModioModID, EModioModChangeType> PreviewMapDef;

/**
 * @docpublic
 * @brief Basic reference data about a dependency relationship to another mod
 * @experimental
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModDependency
{
	GENERATED_BODY();

	/**
	 * @docpublic
	 * @brief The ID of the mod dependency
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	FModioModID ModID;

	/**
	 * @docpublic
	 * @brief The name of the mod dependency
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	FString ModName;

	/**
	 * @docpublic
	 * @brief Unix timestamp of the date the mod was registered
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	FDateTime DateAdded = 0;

	/**
	 * @docpublic
	 * @brief Unix timestamp of the date the mod was updated
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	FDateTime DateUpdated = 0;

	/**
	 * @docpublic
	 * @brief The level at which this dependency sits. When greater than zero (0), it means that this dependency relies
	 *        on additional dependencies.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	uint8 DependencyDepth = 0;

	/**
	 * @docpublic
	 * @brief Media data related to the mod logo
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	FModioLogo Logo;

	/**
	 * @docpublic
	 * @brief Information about the mod's most recent public release
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	FModioFileMetadata FileInfo;

	/**
	 * @docpublic
	 * @brief The current ModStatus on the server: Accepted, NotAccepted, or Deleted.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	EModioModServerSideStatus Status = EModioModServerSideStatus::NotAccepted;

	/**
	 * @docpublic
	 * @brief The visibility status of the mod, default to Public
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependency")
	EModioObjectVisibilityFlags Visibility = EModioObjectVisibilityFlags::Public;
};

/**
 * @docpublic
 * @brief Strong type struct to wrap multiple ModDependency indexed by a paged result
 * @experimental
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModDependencyList
{
	GENERATED_BODY();

	/**
	 * @docpublic
	 * @brief Stored property for a paged result, which provides context on the dependency list
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependencyList")
	FModioPagedResult PagedResult;

	/**
	 * @docpublic
	 * @brief Stored property for the dependency list
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependencyList")
	TArray<FModioModDependency> InternalList;

	/**
	 * @docpublic
	 * @brief Total size of all the dependency files in bytes.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependencyList")
	FModioUnsigned64 TotalFilesize {};

	/**
	 * @docpublic
	 * @brief Total size of the uncompressed dependency files in bytes.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|ModDependencyList")
	FModioUnsigned64 TotalFilesizeUncompressed {};
};

/**
 * @docpublic
 * @brief Strong type struct to wrap a ModDependencyList data as an optional value
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModDependencyList
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioOptionalModDependencyList() = default;

	/**
	 * @docpublic
	 * @brief Convenience constructor that has a list of mod dependencies
	 * @param ModDependencies An optional value that contains mod dependencies
	 */
	FModioOptionalModDependencyList(TOptional<FModioModDependencyList>&& ModDependencies);

	/**
	 * @docpublic
	 * @brief Stored optional ModioModDependencyList
	 */
	TOptional<FModioModDependencyList> Internal;
};