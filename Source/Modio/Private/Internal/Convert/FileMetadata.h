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
#include "Internal/ModioConvert.h"
#include "Internal/ModioPrivateDefines.h"
#include "ModioSDK.h"
#include "Types/ModioFileMetadata.h"

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

FORCEINLINE FModioFileMetadata ToUnreal(const Modio::FileMetadata& FileMetadata)
{
	FModioFileMetadata Out;
	Out.MetadataId = ToUnreal(FileMetadata.MetadataId);
	Out.ModId = ToUnreal(FileMetadata.ModId);
	Out.DateAdded = ToUnrealDateTime(FileMetadata.DateAdded);
	Out.CurrentVirusScanStatus = ToUnreal(FileMetadata.CurrentVirusScanStatus);
	Out.CurrentVirusStatus = ToUnreal(FileMetadata.CurrentVirusStatus);
	Out.Filesize = ToUnreal(FileMetadata.Filesize);
	Out.Filename = ToUnreal(FileMetadata.Filename);
	Out.Version = ToUnreal(FileMetadata.Version);
	Out.Changelog = ToUnreal(FileMetadata.Changelog);
	Out.MetadataBlob = FString(FileMetadata.MetadataBlob.c_str());
	return Out;
}
