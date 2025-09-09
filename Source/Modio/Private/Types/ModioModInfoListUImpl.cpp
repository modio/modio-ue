/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Types/ModioModInfoList.h"

#include "Internal/Convert/ModInfo.h"

#include "Internal/Convert/List.h"
#include "ModioSDK.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModInfoList)

template const TArray<FModioModInfo>& FModioList<TArray, FModioModInfo>::GetRawList() const;

FModioModInfoList::FModioModInfoList(const Modio::ModInfoList& ModInfoList)
	: FModioPagedResult(ModInfoList)
//    FModioList(ToUnrealList<TArray, FModioModInfo>(ModInfoList))
{
    TArray<FModioModInfo> Result;

    Result.Reserve(size_t(ModInfoList.Size()));
    for (const Modio::ModInfo& It : ModInfoList)
    {
        Result.Emplace(ToUnreal(It));
    }

    InternalList = Result;
}

FModioModInfoList::FModioModInfoList(const FModioPagedResult& PagedResult, TArray<FModioModInfo>&& ModInfoList)
	: FModioPagedResult(PagedResult),
	  FModioList(MoveTemp(ModInfoList))
{}

FModioOptionalModInfoList::FModioOptionalModInfoList(TOptional<FModioModInfoList>&& ModInfoList)
	: Internal(MoveTemp(ModInfoList))
{}
