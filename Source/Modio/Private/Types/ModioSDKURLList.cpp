#include "Types/ModioURLList.h"
#include "Internal/ModioConvert.h"

FModioYoutubeURLList::FModioYoutubeURLList(const Modio::YoutubeURLList& URLList)
	: FModioList(ToUnrealList<TArray, FString>(URLList))
{}

FModioSketchfabURLList::FModioSketchfabURLList(const Modio::SketchfabURLList& URLList)
	: FModioList(ToUnrealList<TArray, FString>(URLList))
{}
