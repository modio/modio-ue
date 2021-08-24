#pragma once

#include "Internal/ModioPrivateDefines.h"
#include "ModioSDK.h"
#include "Types/ModioCommonTypes.h"
// clang-format off
#include "ModioFileMetadata.generated.h"
// clang-format on

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

	FModioFileMetadata() = default;
	FModioFileMetadata(const Modio::FileMetadata& FileMetadata);

	/** @brief Unique modfile id. */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata")
	FModioFileMetadataID MetadataId;

	/** @brief Unique mod id. */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata")
	FModioModID ModId;

	/** @brief Unix timestamp of date file was added. */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata")
	FDateTime DateAdded;

	/**
	 * @brief Current virus scan status of the file. For newly added files that have yet to be scanned this field
	 * will change frequently until a scan is complete
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata|Virus")
	EModioVirusScanStatus CurrentVirusScanStatus;

	/** @brief Was a virus detected? */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata|Virus")
	EModioVirusStatus CurrentVirusStatus;

	/** @brief Size of the file in bytes. */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata|File")
	int64 Filesize;

	/** @brief Filename including extension. */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata|File")
	FString Filename;

	/** @brief Release version this file represents. */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata|Version")
	FString Version;

	/** @brief Changelog for the file. */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata|Version")
	FString Changelog;

	/** @brief Metadata stored by the game developer for this file. */
	UPROPERTY(BlueprintReadOnly, Category = "FileMetadata|Metadata")
	FString MetadataBlob;
};

#pragma region ToUnreal implementation
FORCEINLINE FModioFileMetadata ToUnreal(const Modio::FileMetadata& FileMetadata)
{
	return FModioFileMetadata(FileMetadata);
}

MODIO_BEGIN_CONVERT_SWITCHES
FORCEINLINE EModioVirusScanStatus ToUnreal(Modio::FileMetadata::VirusScanStatus VirusScanStatus)
{
	switch (VirusScanStatus)
	{
		case Modio::FileMetadata::VirusScanStatus::NotScanned:
			return EModioVirusScanStatus::NotScanned;
		case Modio::FileMetadata::VirusScanStatus::ScanComplete:
			return EModioVirusScanStatus::ScanComplete;
		case Modio::FileMetadata::VirusScanStatus::InProgress:
			return EModioVirusScanStatus::InProgress;
		case Modio::FileMetadata::VirusScanStatus::TooLargeToScan:
			return EModioVirusScanStatus::TooLargeToScan;
		case Modio::FileMetadata::VirusScanStatus::FileNotFound:
			return EModioVirusScanStatus::FileNotFound;
		case Modio::FileMetadata::VirusScanStatus::ErrorScanning:
			return EModioVirusScanStatus::ErrorScanning;
	}

	return EModioVirusScanStatus::NotScanned;
}

FORCEINLINE EModioVirusStatus ToUnreal(Modio::FileMetadata::VirusStatus VirusStatus)
{
	switch (VirusStatus)
	{
		case Modio::FileMetadata::VirusStatus::NoThreat:
			return EModioVirusStatus::NoThreat;
		case Modio::FileMetadata::VirusStatus::Malicious:
			return EModioVirusStatus::Malicious;
	}

	return EModioVirusStatus::NoThreat;
}
MODIO_END_CONVERT_SWITCHES
#pragma endregion
