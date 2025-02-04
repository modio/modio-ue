/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Types/ModioTokenPackList.h"

#include "Internal/Convert/List.h"
#include "ModioSDK.h"

template const TArray<FModioTokenPack>& FModioList<TArray, FModioTokenPack>::GetRawList() const;

FModioTokenPackList::FModioTokenPackList(const FModioPagedResult& PagedResult, TArray<FModioTokenPack>&& TokenPackList)
	: FModioPagedResult(PagedResult),
	  FModioList(MoveTemp(TokenPackList))
{}

FModioTokenPackList::FModioTokenPackList(TArray<FModioTokenPack> TokenPackList)
	: FModioList(MoveTemp(TokenPackList))
{}

FModioOptionalTokenPackList::FModioOptionalTokenPackList(TOptional<FModioTokenPackList>&& TokenPackList)
	: Internal(MoveTemp(TokenPackList))
{}
