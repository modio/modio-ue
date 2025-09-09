/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once
#include "Types/ModioCommonTypes.h"

#include "ModioCreateSourceFileParams.generated.h"

/**
 * @docpublic
 * @brief Strong type for the parameters needed to create a mod file
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioCreateSourceFileParams
{
	GENERATED_BODY()

	/**
	* @docpublic
	* @brief Path to a directory to treat as the root of the mod source. The SDK will compress all contents of
	* this folder into a .zip archive, with a relative path based on this directory. This directory will not be
	* created itself, the contents will exist at the top level of the archive.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|CreateSourceFileParams")
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
	 * @brief Any extra information needed to provide
	 */
	TOptional<FString> MetadataBlob;

	/**
	 * @docpublic
	 * @brief List of platforms this mod file supports
	 */
	TOptional<TArray<EModioModfilePlatform>> ModfilePlatforms;
};