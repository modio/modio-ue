#pragma once

#include "ModioSDK.h"

// clang-format off
#include "ModioPagedResult.generated.h"
// clang-format on

/** Base class for all api endpoints that can return paged results */
USTRUCT(BlueprintType)
struct MODIO_API FModioPagedResult
{
	GENERATED_BODY()

	FModioPagedResult() = default;
	FModioPagedResult(const Modio::PagedResult& Other);
	FModioPagedResult(int32 ResultOffset, int32 PageSize, int32 TotalResultCount, int32 ResultCount);

	inline int32 GetPageIndex() const
	{
		return PageIndex;
	}
	inline int32 GetPageSize() const
	{
		return PageSize;
	}

	inline int32 GetTotalResultCount() const
	{
		return TotalResultCount;
	}

	inline int32 GetResultCount() const
	{
		return ResultCount;
	}
protected:
	UPROPERTY(BlueprintReadOnly,Category="mod.io|PagedResult|Page")
	int32 PageIndex;

	UPROPERTY(BlueprintReadOnly,Category="mod.io|PagedResult|Page")
	int32 PageSize;

	UPROPERTY(BlueprintReadOnly,Category="mod.io|PagedResult|Page")
	int32 PageCount;

	UPROPERTY(BlueprintReadOnly,Category="mod.io|PagedResult")
	int32 TotalResultCount;

	UPROPERTY(BlueprintReadOnly,Category="mod.io|PagedResult")
	int32 ResultCount;
};

static_assert(sizeof(FModioPagedResult) == sizeof(Modio::PagedResult), "Modio::PagedResult wrapper size mismatch");
