#pragma once

#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "ModioSDK.h"
#include "Types/ModioList.h"

// clang-format off
#include "ModioURLList.generated.h"
// clang-format on

#if CPP
struct MODIO_API FModioYoutubeURLList : public FModioList<TArray, FString>
{
	FModioYoutubeURLList() = default;
	FModioYoutubeURLList(const Modio::YoutubeURLList& URLList);
};

struct MODIO_API FModioSketchfabURLList : public FModioList<TArray, FString>
{
	FModioSketchfabURLList() = default;
	FModioSketchfabURLList(const Modio::SketchfabURLList& URLList);
};
#else
// Blueprint support
struct FModioURLListInternal
{
	TArray<FString> InternalList;
};

USTRUCT(NoExport, BlueprintType)
struct FModioYoutubeURLList
{
	FModioURLListInternal InternalList;
};

USTRUCT(NoExport, BlueprintType)
struct FModioSketchfabURLList
{
	FModioURLListInternal InternalList;
};
#endif

#pragma region ToUnreal implementation
FORCEINLINE FModioYoutubeURLList ToUnreal(const Modio::YoutubeURLList& UrlList)
{
	return FModioYoutubeURLList(UrlList);
}

FORCEINLINE FModioSketchfabURLList ToUnreal(const Modio::SketchfabURLList& UrlList)
{
	return FModioSketchfabURLList(UrlList);
}
#pragma endregion