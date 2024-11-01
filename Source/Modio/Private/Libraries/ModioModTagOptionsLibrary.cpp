/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "Libraries/ModioModTagOptionsLibrary.h"

const TArray<FModioModTagInfo>& UModioModTagOptionsLibrary::GetTags(const FModioModTagOptions& ModTags)
{
	return ModTags.GetRawList();
}

const FModioPagedResult& UModioModTagOptionsLibrary::GetPagedResult(const FModioModTagOptions& ModTags)
{
	return ModTags;
}
