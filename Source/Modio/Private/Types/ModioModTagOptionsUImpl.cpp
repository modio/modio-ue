/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Types/ModioModTagOptions.h"

#include "Internal/Convert/ModTagLocalization.h"
#include "Internal/Convert/ModTagInfo.h"

#include "Internal/Convert/List.h"
#include "ModioSDK.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModTagOptions)

template const TArray<FModioModTagInfo>& FModioList<TArray, FModioModTagInfo>::GetRawList() const;

FModioModTagOptions::FModioModTagOptions(const Modio::ModTagOptions& ModTagList)
	: FModioPagedResult(ModTagList)
//    FModioList(ToUnrealList<TArray, FModioModTagInfo>(ModInfoList))
{
    TArray<FModioModTagInfo> Result;

    Result.Reserve(ModTagList.Size());
    for (const Modio::ModTagInfo& It : ModTagList)
    {
        Result.Emplace(ToUnreal(It));
    }

    InternalList = Result;
}

FModioOptionalModTagOptions::FModioOptionalModTagOptions(TOptional<FModioModTagOptions>&& ModTagOptions)
	: Internal(MoveTemp(ModTagOptions))
{}
