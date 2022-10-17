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

#include "ModioPagedResult.generated.h"

namespace Modio
{
	class PagedResult;
}

/** 
* Strong type struct to acknowledge the position of a result relative to the API endpoints results
**/
USTRUCT(BlueprintType)
struct MODIO_API FModioPagedResult
{
	GENERATED_BODY()

	/**
	* Default constructor without parameters
	**/
	FModioPagedResult() = default;
	
	/**
	* Constructor that uses a paged result to initialize an instance
	* @param Other The paged result to use in the construction of this struct
	**/
	FModioPagedResult(const Modio::PagedResult& Other);
	
	/**
	* Constructor with an offset, size and result counts to create a paged result
	* @param ResultOffset The offset relative to the total result count
	* @param PageSize The size for a set of elements
	* @param TotalResultCount How many elements will be in the search result
	* @param ResultCount How many elements are in this result
	**/
	FModioPagedResult(int32 ResultOffset, int32 PageSize, int32 TotalResultCount, int32 ResultCount);

	/**
	* Retrive the page index
	* @result Integer value with the index
	**/
	int32 GetPageIndex() const
	{
		return PageIndex;
	}

	/**
	* Retrive the page size
	* @result Integer value with the size
	**/
	int32 GetPageSize() const
	{
		return PageSize;
	}
	
	/**
	* Retrive the total count of results
	* @result Integer value with the total amount of results
	**/
	int32 GetTotalResultCount() const
	{
		return TotalResultCount;
	}
	
	/**
	* Retrive the page count
	* @result Integer value with this page count
	**/
	int32 GetResultCount() const
	{
		return ResultCount;
	}

protected:
	/**
	* Stored property of referenced page index
	**/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult|Page")
	int32 PageIndex;

	/**
	* Stored property of referenced page size
	**/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult|Page")
	int32 PageSize;

	/**
	* Stored property of referenced page count
	**/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult|Page")
	int32 PageCount;

	/**
	* Stored property of the result total count
	**/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult")
	int32 TotalResultCount;

	/**
	* Stored property of the result count
	**/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult")
	int32 ResultCount;
};
