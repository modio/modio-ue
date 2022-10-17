/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Types/ModioList.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioPagedResult.h"

#include "ModioModInfoList.generated.h"


namespace Modio
{
	class ModInfoList;
}

#if CPP
/**
* Struct to bring forward a native version of FModioModInfoList 
**/
struct MODIO_API FModioModInfoList : public FModioPagedResult, public FModioList<TArray, FModioModInfo>
{
	/**
	* Default constructor without parameters
	**/
	FModioModInfoList() = default;

	/**
	* Constructor that takes an array mod info and a index of paged results
	**/
	FModioModInfoList(const FModioPagedResult& PagedResult, TArray<FModioModInfo>&& ModInfoList);

	/**
	* Constructor that takes a mod info list to store in this struct
	**/
	FModioModInfoList(const class Modio::ModInfoList& ModInfoList);
};

#else
/** 
* Strong type struct to wrap multiple ModInfo indexed by a paged result 
**/
USTRUCT(NoExport, BlueprintType)
struct MODIO_API FModioModInfoList
{
	/**
	* A paged result property
	**/
	UPROPERTY()
	FModioPagedResult PagedResult;

	/**
	* Arrray of mod tag info
	**/
	UPROPERTY()
	TArray<FModioModInfo> InternalList;
};

#endif

/**
* Struct to wrap ModInfoList into an optional parameter
**/
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalModInfoList
{
	GENERATED_BODY()
	
	/**
	* Default constructor without parameters
	**/
	FModioOptionalModInfoList() = default;
	
	/**
	* Constructor with a mod info list parameter to initialize an instance
	* @param ModInfoList Optional value of a FModioModInfoList
	**/
	FModioOptionalModInfoList(TOptional<FModioModInfoList>&& ModInfoList);

	/**
	* Stored optional ModioModInfoList
	**/
	TOptional<FModioModInfoList> Internal;
};
