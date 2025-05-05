/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "Types/ModioPagedResult.h"
#include "ModioSDK.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioPagedResult)

FModioPagedResult::FModioPagedResult(const Modio::PagedResult& PagedResult)
	: PageIndex(PagedResult.GetPageIndex()),
	  PageSize(PagedResult.GetPageSize()),
	  PageCount(PagedResult.GetPageCount()),
	  TotalResultCount(PagedResult.GetTotalResultCount()),
	  ResultCount(PagedResult.GetResultCount())
{}

FModioPagedResult::FModioPagedResult(int32 ResultOffset, int32 PageSize, int32 TotalResultCount, int32 ResultCount)
	: PageIndex( FMath::FloorToInt( ResultOffset / (float) PageSize) ),
	  PageSize(PageSize),
	  PageCount(FMath::CeilToInt(TotalResultCount / (float) PageSize) ),
	  TotalResultCount(TotalResultCount),
	  ResultCount(ResultCount)
{}
