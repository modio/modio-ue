#include "Types/ModioFileMetadata.h"
#include "Internal/ModioConvert.h"

FModioFileMetadata::FModioFileMetadata(const Modio::FileMetadata& ModInfo)
	: MetadataId(ToUnreal(ModInfo.MetadataId)),
	  ModId(ToUnreal(ModInfo.ModId)),
	  DateAdded(ToUnrealDateTime(ModInfo.DateAdded)),
	  CurrentVirusScanStatus(ToUnreal(ModInfo.CurrentVirusScanStatus)),
	  CurrentVirusStatus(ToUnreal(ModInfo.CurrentVirusStatus)),
	  Filesize(ToUnreal(ModInfo.Filesize)),
	  Filename(ToUnreal(ModInfo.Filename)),
	  Version(ToUnreal(ModInfo.Version)),
	  Changelog(ToUnreal(ModInfo.Changelog)),
	  MetadataBlob(FString(ModInfo.MetadataBlob.c_str()))
{}
