#include "Types/ModioModInfoList.h"

FModioModInfoList::FModioModInfoList(const Modio::ModInfoList& ModInfoList)
	: FModioPagedResult(ModInfoList),
	  FModioList(ToUnrealList<TArray, FModioModInfo>(ModInfoList))
{}

FModioModInfoList::FModioModInfoList(const FModioPagedResult& PagedResult, TArray<FModioModInfo>&& ModInfoList)
	: FModioPagedResult(PagedResult),
	  FModioList(MoveTemp(ModInfoList))
{}

FModioOptionalModInfoList::FModioOptionalModInfoList(TOptional<FModioModInfoList>&& ModInfoList)
	: Internal(MoveTemp(ModInfoList))
{}
