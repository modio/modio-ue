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

#include "ModioPagedResult.generated.h"

namespace Modio
{
	class PagedResult;
}

/** Base class for all api endpoints that can return paged results */
USTRUCT(BlueprintType)
struct MODIO_API FModioPagedResult
{
	GENERATED_BODY()

	FModioPagedResult() = default;
	FModioPagedResult(const Modio::PagedResult& Other);
	FModioPagedResult(int32 ResultOffset, int32 PageSize, int32 TotalResultCount, int32 ResultCount);

	int32 GetPageIndex() const
	{
		return PageIndex;
	}
	int32 GetPageSize() const
	{
		return PageSize;
	}

	int32 GetTotalResultCount() const
	{
		return TotalResultCount;
	}

	int32 GetResultCount() const
	{
		return ResultCount;
	}

protected:
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult|Page")
	int32 PageIndex;

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult|Page")
	int32 PageSize;

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult|Page")
	int32 PageCount;

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult")
	int32 TotalResultCount;

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult")
	int32 ResultCount;
};
