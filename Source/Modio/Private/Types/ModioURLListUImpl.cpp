/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Internal/Convert/List.h"
#include "Types/ModioURLList.h"

#include "ModioSDK.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioURLList)

template const TArray<FString>& FModioList<TArray, FString>::GetRawList() const;

FModioYoutubeURLList::FModioYoutubeURLList(const Modio::YoutubeURLList& URLList)
	: FModioList(ToUnrealList<TArray, FString>(URLList))
{}

FModioSketchfabURLList::FModioSketchfabURLList(const Modio::SketchfabURLList& URLList)
	: FModioList(ToUnrealList<TArray, FString>(URLList))
{}
