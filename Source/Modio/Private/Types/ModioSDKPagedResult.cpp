#include "Types/ModioPagedResult.h"

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
