/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Types/ModioModTagOptions.h"

#include "Internal/Convert/ModTagInfo.h"

#include "Internal/Convert/List.h"
#include "ModioSDK.h"

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
