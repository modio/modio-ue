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

#include "Types/ModioCommonTypes.h"

#include "ModioModProgressInfo.generated.h"

namespace Modio
{
	class ModProgressInfo;
}

/** @brief Class representing the progress of a mod installation or update */
USTRUCT(BlueprintType)
struct MODIO_API FModioModProgressInfo
{
	GENERATED_BODY()

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
struct MODIO_API FModioOptionalModProgressInfo
{
	GENERATED_BODY()

	TOptional<FModioModProgressInfo> Internal;
};

