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

#include "Types/ModioList.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioPagedResult.h"

#include "ModioModInfoList.generated.h"


namespace Modio
{
	class ModInfoList;
}

#if CPP
// Native version of FModioModInfoList
struct MODIO_API FModioModInfoList : public FModioPagedResult, public FModioList<TArray, FModioModInfo>
{
	FModioModInfoList() = default;
	FModioModInfoList(const FModioPagedResult& PagedResult, TArray<FModioModInfo>&& ModInfoList);
	FModioModInfoList(const class Modio::ModInfoList& ModInfoList);
};

#else
// Blueprint mirror of FModioModInfoList that's flattened as blueprints don't understand either template classes or
// multiple inheritance
USTRUCT(NoExport, BlueprintType)
struct MODIO_API FModioModInfoList
{
	UPROPERTY()
	FModioPagedResult PagedResult;

	UPROPERTY()
	TArray<FModioModInfo> InternalList;
};

#endif

USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModInfoList
{
	GENERATED_BODY()

	FModioOptionalModInfoList() = default;
	FModioOptionalModInfoList(TOptional<FModioModInfoList>&& ModInfoList);

	TOptional<FModioModInfoList> Internal;
};
