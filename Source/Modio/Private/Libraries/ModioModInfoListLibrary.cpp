/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "Libraries/ModioModInfoListLibrary.h"

const TArray<FModioModInfo>& UModioModInfoListLibrary::GetMods(const FModioModInfoList& ModInfoList)
{
	return ModInfoList.GetRawList();
}

const FModioPagedResult& UModioModInfoListLibrary::GetPagedResult(const FModioModInfoList& ModInfoList)
{
	return ModInfoList;
}
