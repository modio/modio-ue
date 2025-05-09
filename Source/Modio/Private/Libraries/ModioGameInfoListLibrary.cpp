/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "Libraries/ModioGameInfoListLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioGameInfoListLibrary)

const TArray<FModioGameInfo>& UModioGameInfoListLibrary::GetGames(const FModioGameInfoList& GameInfoList)
{
	return GameInfoList.GetRawList();
}

const FModioPagedResult& UModioGameInfoListLibrary::GetPagedResult(const FModioGameInfoList& GameInfoList)
{
	return GameInfoList;
}
