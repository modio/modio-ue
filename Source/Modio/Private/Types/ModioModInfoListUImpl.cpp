/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Types/ModioModInfoList.h"

#include "Internal/Convert/ModInfo.h"

#include "Internal/Convert/List.h"
#include "ModioSDK.h"

FModioModInfoList::FModioModInfoList(const Modio::ModInfoList& ModInfoList)
	: FModioPagedResult(ModInfoList)
//    FModioList(ToUnrealList<TArray, FModioModInfo>(ModInfoList))
{
    TArray<FModioModInfo> Result;

    Result.Reserve(ModInfoList.Size());
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
