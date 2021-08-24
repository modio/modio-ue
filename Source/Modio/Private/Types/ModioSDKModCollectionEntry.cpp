#include "Types/ModioModCollectionEntry.h"
#include "Internal/ModioConvert.h"

FModioModCollectionEntry::FModioModCollectionEntry(Modio::ModCollectionEntry&& ModCollectionEntry)
	: ModCollectionEntry(MoveTemp(ModCollectionEntry))
{}

EModioModState FModioModCollectionEntry::GetModState() const
{
	return ToUnreal(ModCollectionEntry.GetModState());
}

FModioModID FModioModCollectionEntry::GetID() const
{
	return ToUnreal(ModCollectionEntry.GetID());
}

const FModioModInfo& FModioModCollectionEntry::GetModProfile() const
{
	if (!CachedModInfo)
	{
		CachedModInfo = ToUnreal(ModCollectionEntry.GetModProfile());
	}
	return CachedModInfo.GetValue();
}

const FString& FModioModCollectionEntry::GetPath() const
{
	if (!CachedPath)
	{
		CachedPath = ToUnreal(ModCollectionEntry.GetPath().string());
	}
	return CachedPath.GetValue();
}
