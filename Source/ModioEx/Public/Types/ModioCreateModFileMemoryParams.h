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

#include "ModioCreateModFileMemoryParams.generated.h"

/**
* Strong type struct to store an on-memory file parameters
**/
USTRUCT(BlueprintType)
struct MODIOEX_API FModioCreateModFileMemoryParams
{
	GENERATED_BODY()

	/**
	* Stored memory of the Mod file
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|FileMemoryParams")
	TArray<uint8> ModMemory;

	/**
	* Optional string to store the version
	**/
	TOptional<FString> VersionString;

	/**
	* Optional string to store the change log
	**/
	TOptional<FString> Changelog;

	/**
	* Optional boolean to store the active release status
	**/
	TOptional<bool> bSetAsActiveRelease;

	/**
	* Optional string to store the metadata
	**/
	TOptional<FString> MetadataBlob;
};
