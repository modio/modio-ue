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
#include "Types/ModioCommonTypes.h"

#include "ModioCreateModFileParams.generated.h"

/**
 * @docpublic
 * @brief Strong type for the parameters needed to create a mod file
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioCreateModFileParams
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief The file system path that references the directory with the mod files. The plugin will compress the contents of
	 * this path into a .zip archive.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|CreateModFileParams")
	FString PathToModRootDirectory;

	/**
	 * @docpublic
	 * @brief The version for this mod file
	 */
	TOptional<FString> VersionString;

	/**
	 * @docpublic
	 * @brief Any changes between versions
	 */
	TOptional<FString> Changelog;

	/**
	 * @docpublic
	 * @brief True to set the mod file as an active release for versioning purposes
	 */
	TOptional<bool> bSetAsActiveRelease;

	/**
	 * @docpublic
	 * @brief Any extra information needed to provide
	 */
	TOptional<FString> MetadataBlob;

	/**
	 * @docpublic
	 * @brief List of platforms this mod file supports
	 */
	TOptional<TArray<EModioModfilePlatform>> ModfilePlatforms;
};