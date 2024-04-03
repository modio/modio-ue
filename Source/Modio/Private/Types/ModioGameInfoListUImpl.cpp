/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Types/ModioGameInfoList.h"

#include "Internal/Convert/GameInfo.h"

#include "Internal/Convert/List.h"
#include "ModioSDK.h"

FModioGameInfoList::FModioGameInfoList(const Modio::GameInfoList& GameInfoList)
	: FModioPagedResult(GameInfoList)
{
    TArray<FModioGameInfo> Result;

    Result.Reserve(GameInfoList.Size());
    for (const Modio::GameInfo& It : GameInfoList)
    {
        Result.Emplace(ToUnreal(It));
    }

    InternalList = Result;

}

FModioGameInfoList::FModioGameInfoList(const FModioPagedResult& PagedResult, TArray<FModioGameInfo>&& GameInfoList)
	: FModioPagedResult(PagedResult),
	  FModioList(MoveTemp(GameInfoList))
{}

FModioOptionalGameInfoList::FModioOptionalGameInfoList(TOptional<FModioGameInfoList>&& GameInfoList)
	: Internal(MoveTemp(GameInfoList))
{}