#include "Libraries/ModioModTagOptionsLibrary.h"

const TArray<FModioModTagInfo>& UModioModTagOptionsLibrary::GetTags(const FModioModTagOptions& ModTags)
{
	return ModTags.GetRawList();
}

const FModioPagedResult& UModioModTagOptionsLibrary::GetPagedResult(const FModioModTagOptions& ModTags)
{
	return ModTags;
}
