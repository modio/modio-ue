#pragma once

#include "Containers/UnrealString.h"

// clang-format off
#include "ModioMetadata.generated.h"
// clang-format on

namespace Modio
{
	struct Metadata;
}

USTRUCT(BlueprintType)
struct MODIO_API FModioMetadata
{
	GENERATED_BODY()

	FModioMetadata() = default;
	FModioMetadata(const Modio::Metadata& Metadata);

	/** Key of the metadata */
	UPROPERTY(BlueprintReadOnly, Category = "Metadata")
	FString Key;

	/** Value of the metadata */
	UPROPERTY(BlueprintReadOnly, Category = "Metadata")
	FString Value;
};

#pragma region ToUnreal implementation
FORCEINLINE FModioMetadata ToUnreal(const Modio::Metadata& Metadata)
{
	return FModioMetadata(Metadata);
}
#pragma endregion
