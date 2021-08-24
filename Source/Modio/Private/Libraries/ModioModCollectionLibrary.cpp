#include "Libraries/ModioModCollectionLibrary.h"

EModioModState UModioModCollectionLibrary::GetModState(const FModioModCollectionEntry& Entry)
{
	return Entry.GetModState();
}

FModioModID UModioModCollectionLibrary::GetID(const FModioModCollectionEntry& Entry)
{
	return Entry.GetID();
}

const FModioModInfo& UModioModCollectionLibrary::GetModProfile(const FModioModCollectionEntry& Entry)
{
	return Entry.GetModProfile();
}

const FString& UModioModCollectionLibrary::GetPath(const FModioModCollectionEntry& Entry)
{
	return Entry.GetPath();
}
