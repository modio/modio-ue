#include "Types/ModioModTagInfo.h"
#include "Internal/ModioConvert.h"

FModioModTagInfo::FModioModTagInfo(const Modio::ModTagInfo& ModTagInfo)
	: TagGroupName(ToUnreal(ModTagInfo.TagGroupName)),
	  TagGroupValues(ToUnreal<FString>(ModTagInfo.TagGroupValues)),
	  bAllowMultipleSelection(ToUnreal(ModTagInfo.bAllowMultipleSelection))

{}
