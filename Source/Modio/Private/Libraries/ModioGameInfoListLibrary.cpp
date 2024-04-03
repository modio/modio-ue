/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "Libraries/ModioGameInfoListLibrary.h"

const TArray<FModioGameInfo>& UModioGameInfoListLibrary::GetGames(const FModioGameInfoList& GameInfoList)
{
	return GameInfoList.GetRawList();
}

const FModioPagedResult& UModioGameInfoListLibrary::GetPagedResult(const FModioGameInfoList& GameInfoList)
{
	return GameInfoList;
}
