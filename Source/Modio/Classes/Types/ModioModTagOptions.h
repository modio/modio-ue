#pragma once

#include "ModioSDK.h"
#include "Types/ModioModInfoList.h"
#include "Types/ModioModTagInfo.h"

// clang-format off
#include "ModioModTagOptions.generated.h"
// clang-format on

#if CPP
// Native version of FModioModTagOptions
struct MODIO_API FModioModTagOptions : public FModioPagedResult, public FModioList<TArray, FModioModTagInfo>
{
	FModioModTagOptions() = default;
	FModioModTagOptions(const Modio::ModTagOptions& ModInfoList);
};

#else
// Blueprint mirror of FModioModTagOptions that's flattened as blueprints don't understand either template classes or multiple inheritance
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
struct FModioOptionalModTagOptions
{
	GENERATED_BODY()

	FModioOptionalModTagOptions() = default;
	FModioOptionalModTagOptions(TOptional<FModioModTagOptions>&& ModTagOptions);

	TOptional<FModioModTagOptions> Internal;
};

#pragma region ToUnreal implementation
FORCEINLINE FModioModTagOptions ToUnreal(const Modio::ModTagOptions& ModTagOptions)
{
	return FModioModTagOptions(ModTagOptions);
}
#pragma endregion
