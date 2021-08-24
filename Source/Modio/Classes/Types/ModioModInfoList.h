#pragma once

#include "ModioSDK.h"
#include "Types/ModioList.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioPagedResult.h"
// clang-format off
#include "ModioModInfoList.generated.h"
// clang-format on

#if CPP
// Native version of FModioModInfoList
struct MODIO_API FModioModInfoList : public FModioPagedResult, public FModioList<TArray, FModioModInfo>
{
	FModioModInfoList() = default;
	FModioModInfoList(const FModioPagedResult& PagedResult, TArray<FModioModInfo>&& ModInfoList);
	FModioModInfoList(const Modio::ModInfoList& ModInfoList);
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

#pragma region ToUnreal implementation
FORCEINLINE FModioModInfoList ToModio(const Modio::ModInfoList& ModInfoList)
{
	return FModioModInfoList(ModInfoList);
}
#pragma endregion
