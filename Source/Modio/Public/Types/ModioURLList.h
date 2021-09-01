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

#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "Types/ModioList.h"

#include "ModioURLList.generated.h"

namespace Modio
{
	class YoutubeURLList;
	class SketchfabURLList;
}

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
