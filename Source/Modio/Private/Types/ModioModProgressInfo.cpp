#include "Types/ModioModProgressInfo.h"
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"

FModioModProgressInfo::FModioModProgressInfo(const Modio::ModProgressInfo& ModProgressInfo)
	: TotalDownloadSize(ToUnreal(ModProgressInfo.TotalDownloadSize)),
	  CurrentlyDownloadedBytes(ToUnreal(ModProgressInfo.CurrentlyDownloadedBytes)),
	  TotalExtractedSizeOnDisk(ToUnreal(ModProgressInfo.TotalExtractedSizeOnDisk)),
	  CurrentlyExtractedBytes(ToUnreal(ModProgressInfo.CurrentlyExtractedBytes)),
	  ID(ToUnreal(ModProgressInfo.ID))
{}
