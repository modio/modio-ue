/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
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
