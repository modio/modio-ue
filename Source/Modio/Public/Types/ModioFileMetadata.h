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

#include "Misc/DateTime.h"
#include "ModioUnsigned64.h"
#include "Types/ModioCommonTypes.h"

#include "ModioFileMetadata.generated.h"

/**
 * @docpublic
 * @brief Current state of the scanned file
 */
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

/**
 * @docpublic
 * @brief If the file has been found to be malicious or not
 */
UENUM(BlueprintType)
enum class EModioVirusStatus : uint8
{
	NoThreat = 0,
	Malicious = 1
};

/**
 * @docpublic
 * @brief Metadata for a release archive for a mod
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioFileMetadata
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Unique modfile id.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata")
	FModioFileMetadataID MetadataId;

	/**
	 * @docpublic
	 * @brief Unique mod id.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata")
	FModioModID ModId;

	/**
	 * @docpublic
	 * @brief Unix timestamp of date file was added.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata")
	FDateTime DateAdded;

	/**
	 * @docpublic
	 * @brief Current virus scan status of the file. For newly added files that have yet to be scanned this field
	 * will change frequently until a scan is complete
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|Virus")
	EModioVirusScanStatus CurrentVirusScanStatus {};

	/**
	 * @docpublic
	 * @brief Was a virus detected?
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|Virus")
	EModioVirusStatus CurrentVirusStatus {};

	/**
	 * @docpublic
	 * @brief Size of the file in bytes.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|File")
	FModioUnsigned64 Filesize;

	/**
	 * @docpublic
	 * @brief Total size of all files in the mod after installation.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|File")
	FModioUnsigned64 FilesizeUncompressed;

	/**
	 * @docpublic
	 * @brief Filename including extension.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|File")
	FString Filename;

	/**
	 * @docpublic
	 * @brief Release version this file represents.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|Version")
	FString Version;

	/**
	 * @docpublic
	 * @brief Changelog for the file.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|Version")
	FString Changelog;

	/**
	 * @docpublic
	 * @brief Metadata stored by the game developer for this file.
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "FileMetadata|Metadata")
	FString MetadataBlob;
};