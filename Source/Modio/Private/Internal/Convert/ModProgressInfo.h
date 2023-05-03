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
#include "Internal/ModioConvert.h"
#include "Modio.h"
#include "ModioSDK.h"
#include "Types/ModioModProgressInfo.h"

FORCEINLINE EModioModProgressState ToUnreal(const Modio::ModProgressInfo::EModProgressState In)
{
	switch (In)
	{
		case Modio::ModProgressInfo::EModProgressState::Compressing:
			return EModioModProgressState::Compressing;
		case Modio::ModProgressInfo::EModProgressState::Uploading:
			return EModioModProgressState::Uploading;
		case Modio::ModProgressInfo::EModProgressState::Downloading:
			return EModioModProgressState::Downloading;
		case Modio::ModProgressInfo::EModProgressState::Extracting:
			return EModioModProgressState::Extracting;
		case Modio::ModProgressInfo::EModProgressState::Initializing:
			return EModioModProgressState::Initializing;
	}
	UE_LOG(LogModio, Error, TEXT("Unhandled case in ToUnreal(EModProgressState)"));
	return EModioModProgressState::Initializing;
}

FORCEINLINE FModioModProgressInfo ToUnreal(const Modio::ModProgressInfo& In)
{
	FModioModProgressInfo Out;
	Out.DownloadTotal = In.GetTotalProgress(Modio::ModProgressInfo::EModProgressState::Downloading);
	Out.DownloadCurrent = In.GetCurrentProgress(Modio::ModProgressInfo::EModProgressState::Downloading);

	Out.ExtractTotal = In.GetTotalProgress(Modio::ModProgressInfo::EModProgressState::Extracting);
	Out.ExtractCurrent = In.GetCurrentProgress(Modio::ModProgressInfo::EModProgressState::Extracting);
	Out.CompressTotal = In.GetTotalProgress(Modio::ModProgressInfo::EModProgressState::Compressing);
	Out.CompressCurrent = In.GetCurrentProgress(Modio::ModProgressInfo::EModProgressState::Compressing);

	Out.UploadTotal = In.GetTotalProgress(Modio::ModProgressInfo::EModProgressState::Uploading);
	Out.UploadCurrent = In.GetCurrentProgress(Modio::ModProgressInfo::EModProgressState::Uploading);
	Out.CurrentState = ToUnreal(In.GetCurrentState());

	Out.ID = ToUnreal(In.ID);
	return Out;
}
