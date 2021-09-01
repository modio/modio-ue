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
#include "ModioSDK.h"
#include "Types/ModioModProgressInfo.h"

FORCEINLINE FModioModProgressInfo ToUnreal(const Modio::ModProgressInfo& In)
{
	FModioModProgressInfo Out;
	Out.TotalDownloadSize = ToUnreal(In.TotalDownloadSize);
	Out.CurrentlyDownloadedBytes = ToUnreal(In.CurrentlyDownloadedBytes);
	Out.TotalExtractedSizeOnDisk = ToUnreal(In.TotalExtractedSizeOnDisk);
	Out.ID = ToUnreal(In.ID);
	return Out;
}
