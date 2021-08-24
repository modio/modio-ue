#pragma once

#include "Containers/Array.h"
#include "Containers/UnrealString.h"

// clang-format off
#include "ModioModTagInfo.generated.h"
// clang-format on

namespace Modio
{
	struct ModTagInfo;
}

/** @brief Metadata about a group of tags that can be used for filtering mods */
USTRUCT(BlueprintType)
struct MODIO_API FModioModTagInfo
{
	GENERATED_BODY()

	FModioModTagInfo() = default;
	FModioModTagInfo(const Modio::ModTagInfo& ModTagOptions);

	/** @brief The display name for the tag */
	UPROPERTY(BlueprintReadOnly,Category="mod.io|ModTagInfo")
	FString TagGroupName;

	/** @brief The valid tags the group can have */
	UPROPERTY(BlueprintReadOnly,Category="mod.io|ModTagInfo")
	TArray<FString> TagGroupValues;

	/** @brief True if multiple tags from the group can be used simultaneously */
	UPROPERTY(BlueprintReadOnly,Category="mod.io|ModTagInfo")
	bool bAllowMultipleSelection;
};

#pragma region ToUnreal implementation
FORCEINLINE FModioModTagInfo ToUnreal(const Modio::ModTagInfo& ModTagInfo)
{
	return FModioModTagInfo(ModTagInfo);
}
#pragma endregion
