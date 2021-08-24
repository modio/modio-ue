#include "Libraries/ModioModInfoListLibrary.h"

const TArray<FModioModInfo>& UModioModInfoListLibrary::GetMods(const FModioModInfoList& ModInfoList)
{
	return ModInfoList.GetRawList();
}

const FModioPagedResult& UModioModInfoListLibrary::GetPagedResult(const FModioModInfoList& ModInfoList)
{
	return ModInfoList;
}
