/*
*  Copyright (C) 2024-2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Internal/ModioConvert.h"
#include "Internal/Convert/ModCollectionInfo.h"
#include "Internal/Convert/ModCollectionStats.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModCollection)

FModioModCollectionInfoList::FModioModCollectionInfoList(const FModioPagedResult& PagedResult,
                                                         TArray<FModioModCollectionInfo>&& ModCollectionInfoList)
	: FModioPagedResult(PagedResult),
	  FModioList(MoveTemp(ModCollectionInfoList)) {}

FModioModCollectionInfoList::FModioModCollectionInfoList(const Modio::ModCollectionInfoList& ModCollectionInfoList)
	: FModioPagedResult(ModCollectionInfoList)
{
	TArray<FModioModCollectionInfo> Result{};

	Result.Reserve(ModCollectionInfoList.Size());
	for (const Modio::ModCollectionInfo& It : ModCollectionInfoList)
	{
		Result.Emplace(ToUnreal(It));
	}

	InternalList = Result;
}

FModioOptionalModCollectionInfoList::FModioOptionalModCollectionInfoList(
	TOptional<FModioModCollectionInfoList>&& ModCollectionInfoList)
	: Internal(MoveTemp(ModCollectionInfoList)) {}