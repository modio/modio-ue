#include "Types/ModioModTagOptions.h"

FModioModTagOptions::FModioModTagOptions(const Modio::ModTagOptions& ModInfoList)
	: FModioPagedResult(ModInfoList),
	  FModioList(ToUnrealList<TArray, FModioModTagInfo>(ModInfoList))
{}

FModioOptionalModTagOptions::FModioOptionalModTagOptions(TOptional<FModioModTagOptions>&& ModTagOptions)
	: Internal(MoveTemp(ModTagOptions))
{}
