#pragma once

#include "Containers/UnrealString.h"
#include "ModioSDK.h"

// clang-format off
#include "ModioModTag.generated.h"
// clang-format on

USTRUCT(BlueprintType)
struct MODIO_API FModioModTag
{
	GENERATED_BODY()

	FModioModTag() = default;
	FModioModTag(const Modio::ModTag& ModTag);

	UPROPERTY(BlueprintReadOnly, Category = "ModTag")
	FString Tag;
};

#pragma region ToUnreal implementation
FORCEINLINE FModioModTag ToUnreal(const Modio::ModTag& ModTag)
{
	return FModioModTag(ModTag);
}
#pragma endregion
