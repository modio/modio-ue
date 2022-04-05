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

#include "ModioCreateModFileMemoryParams.generated.h"

USTRUCT(BlueprintType)
struct MODIOEX_API FModioCreateModFileMemoryParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|FileMemoryParams")
	TArray<uint8> ModMemory;

	TOptional<FString> VersionString;

	TOptional<FString> Changelog;

	TOptional<bool> bSetAsActiveRelease;

	TOptional<FString> MetadataBlob;
};
