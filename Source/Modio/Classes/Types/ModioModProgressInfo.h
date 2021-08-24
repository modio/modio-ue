#pragma once

#include "ModioSDK.h"
#include "Types/ModioCommonTypes.h"

// clang-format off
#include "ModioModProgressInfo.generated.h"
// clang-format on

namespace Modio
{
	class ModProgressInfo;
}

/** @brief Class representing the progress of a mod installation or update */
USTRUCT(BlueprintType)
struct MODIO_API FModioModProgressInfo
{
	GENERATED_BODY()

	FModioModProgressInfo() = default;
	FModioModProgressInfo(const Modio::ModProgressInfo& ModProgressInfo);

	/** @brief Total size of the downloaded file */
	UPROPERTY(BlueprintReadOnly,Category="ModProgressInfo")
	int64 TotalDownloadSize;

	/** @brief Current amount downloaded in bytes */
	UPROPERTY(BlueprintReadOnly,Category="ModProgressInfo|Current")
	int64 CurrentlyDownloadedBytes;

	/** @brief Total size on disk when fully extracted */
	UPROPERTY(BlueprintReadOnly,Category="ModProgressInfo")
	int64 TotalExtractedSizeOnDisk;

	/** @brief Amount of data currently extracted */
	UPROPERTY(BlueprintReadOnly,Category="ModProgressInfo|Current")
	int64 CurrentlyExtractedBytes;

	/** @brief The mod ID of the mod being processed */
	UPROPERTY(BlueprintReadOnly,Category="ModProgressInfo")
	FModioModID ID;
};

USTRUCT(BlueprintType)
struct FModioOptionalModProgressInfo
{
	GENERATED_BODY()

	TOptional<FModioModProgressInfo> Internal;
};

#pragma region ToUnreal implementation
FORCEINLINE FModioModProgressInfo ToUnreal(const Modio::ModProgressInfo& ModProgressInfo)
{
	return FModioModProgressInfo(ModProgressInfo);
}
#pragma region ToUnreal implementation
