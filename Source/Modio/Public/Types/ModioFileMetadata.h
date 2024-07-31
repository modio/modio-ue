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

#include "Types/ModioCommonTypes.h"
#include "Misc/DateTime.h"

#include "ModioFileMetadata.generated.h"

/** @brief Current state of the scanned file */
UENUM(BlueprintType)
enum class EModioVirusScanStatus : uint8
{
	NotScanned = 0,
	ScanComplete = 1,
	InProgress = 2,
	TooLargeToScan = 3,
	FileNotFound = 4,
	ErrorScanning = 5
};

/** @brief If the file has been found to be malicious or not */
UENUM(BlueprintType)
enum class EModioVirusStatus : uint8
{
	NoThreat = 0,
	Malicious = 1
};

/** @brief Metadata for a release archive for a mod */
USTRUCT(BlueprintType)
struct MODIO_API FModioFileMetadata
{
	GENERATED_BODY()

	/** @brief Unique modfile id. */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata")
	FModioFileMetadataID MetadataId;

	/** @brief Unique mod id. */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata")
	FModioModID ModId;

	/** @brief Unix timestamp of date file was added. */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata")
	FDateTime DateAdded;

	/**
	 * @brief Current virus scan status of the file. For newly added files that have yet to be scanned this field
	 * will change frequently until a scan is complete
	 **/
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|Virus")
	EModioVirusScanStatus CurrentVirusScanStatus {};

	/** @brief Was a virus detected? */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|Virus")
	EModioVirusStatus CurrentVirusStatus {};

	/** @brief Size of the file in bytes. */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|File")
	int64 Filesize {};

	/** @brief Total size of all files in the mod after installation. */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|File")
	int64 FilesizeUncompressed {};

	/** @brief Filename including extension. */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|File")
	FString Filename;

	/** @brief Release version this file represents. */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|Version")
	FString Version;

	/** @brief Changelog for the file. */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|Version")
	FString Changelog;

	/** @brief Metadata stored by the game developer for this file. */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|Metadata")
	FString MetadataBlob;
};
