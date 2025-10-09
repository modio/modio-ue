/* 
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "Libraries/ModioModCollectionInfoListLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModCollectionInfoListLibrary)

const TArray<FModioModCollectionInfo>& UModioModCollectionInfoListLibrary::GetCollections(
	const FModioModCollectionInfoList& ModCollectionInfoList)
{
	return ModCollectionInfoList.GetRawList();
}

const FModioPagedResult& UModioModCollectionInfoListLibrary::GetPagedResult(
	const FModioModCollectionInfoList& ModCollectionInfoList)
{
	return ModCollectionInfoList;
}