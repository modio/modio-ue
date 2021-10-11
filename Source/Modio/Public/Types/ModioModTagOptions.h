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
#include "Types/ModioModTagInfo.h"
#include "Types/ModioPagedResult.h"

#include "ModioModTagOptions.generated.h"

namespace Modio
{
	class ModTagOptions;
}

#if CPP
// Native version of FModioModTagOptions
struct MODIO_API FModioModTagOptions : public FModioPagedResult, public FModioList<TArray, FModioModTagInfo>
{
	FModioModTagOptions() = default;
	FModioModTagOptions(const Modio::ModTagOptions& ModInfoList);
};

#else
// Blueprint mirror of FModioModTagOptions that's flattened as blueprints don't understand either template classes or
// multiple inheritance
USTRUCT(NoExport, BlueprintType)
struct MODIO_API FModioModTagOptions
{
	UPROPERTY()
	FModioPagedResult PagedResult;

	UPROPERTY()
	TArray<FModioModTagInfo> InternalList;
};

#endif

USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModTagOptions
{
	GENERATED_BODY()

	FModioOptionalModTagOptions() = default;
	FModioOptionalModTagOptions(TOptional<FModioModTagOptions>&& ModTagOptions);

	TOptional<FModioModTagOptions> Internal;
};
