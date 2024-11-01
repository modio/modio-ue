/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Types/ModioCommonTypes.h"
#include "Types/ModioUnsigned64.h"

#include "ModioModProgressInfo.generated.h"

namespace Modio
{
	class ModProgressInfo;
}

UENUM(BlueprintType)
enum class EModioModProgressState : uint8
{
	Initializing, // Download information is being retrieved from mod.io servers
	Downloading, // Mod archive is downloading from mod.io servers
	Extracting, // Mod archive is downloaded and now extracting
	Compressing, // Mod archive is being compressed from files on disk
	Uploading // Mod archive is uploading to mod.io servers
};

/**
 * Strong type struct representing the progress of a mod installation or update
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioModProgressInfo
{
	GENERATED_BODY()

	friend class UModioUIEditorPreviewDataLibrary;

	/**
	 * The mod ID of the mod being processed
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "ModProgressInfo")
	FModioModID ID;

	EModioModProgressState GetCurrentState() const
	{
		return CurrentState;
	}

	/// @brief Retrieves the progress value for the specified state. CurrentProgress == TotalProgress for states
	/// which have completed, for example if a mod is currently Extracting, then passing in Downloading would give
	/// you a value equal to the total download size
	/// @param State which state to query progress information for
	/// @return Modio::FileSize for current progress in bytes
	FModioUnsigned64 GetCurrentProgress(EModioModProgressState State) const
	{
		switch (State)
		{
			case EModioModProgressState::Downloading:
				return FModioUnsigned64(DownloadCurrent);
			case EModioModProgressState::Extracting:
				return FModioUnsigned64(ExtractCurrent);
			case EModioModProgressState::Uploading:
				return FModioUnsigned64(UploadCurrent);
			case EModioModProgressState::Compressing:
				return FModioUnsigned64(CompressCurrent);
			default:
				return FModioUnsigned64(0);
		}
	}

	/// @brief Retrieves the total amount of progress required for the specified state.
	/// @param State which state to query total progress for
	/// @return Modio::FileSize for total progress in bytes
	FModioUnsigned64 GetTotalProgress(EModioModProgressState State) const
	{
		switch (State)
		{
			case EModioModProgressState::Downloading:
				return FModioUnsigned64(DownloadTotal);
			case EModioModProgressState::Extracting:
				return FModioUnsigned64(ExtractTotal);
			case EModioModProgressState::Uploading:
				return FModioUnsigned64(UploadTotal);
			case EModioModProgressState::Compressing:
				return FModioUnsigned64(CompressTotal);
			default:
				return FModioUnsigned64(0);
		}
	}

private:
	friend FModioModProgressInfo ToUnreal(const Modio::ModProgressInfo& In);

	uint64_t DownloadCurrent;
	uint64_t DownloadTotal;

	uint64_t ExtractCurrent;
	uint64_t ExtractTotal;

	uint64_t CompressCurrent;
	uint64_t CompressTotal;

	uint64_t UploadCurrent;
	uint64_t UploadTotal;

	EModioModProgressState CurrentState;
};

/**
 * Struct strong type to wrap an optional value for the ModProgressInfo
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModProgressInfo
{
	GENERATED_BODY()

	/**
	 * Stored property for the optional ModProgressInfo
	 **/
	TOptional<FModioModProgressInfo> Internal;
};
