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
	 * Retrieve the index of this page
	 * @result Integer value with the index
	 **/
	int32 GetPageIndex() const
	{
		return PageIndex;
	}

	/**
	 * Retrieve the amount of results per page
	 * @result Integer value with the size
	 **/
	int32 GetPageSize() const
	{
		return PageSize;
	}

	/**
	 * Retrieve the total amount of pages
	 * @result Integer value with the page count
	 **/
	int32 GetPageCount() const
	{
		return PageCount;
	}

	/**
	 * Retrieve the total amount of results
	 * @result Integer value with the total result count
	 **/
	int32 GetTotalResultCount() const
	{
		return TotalResultCount;
	}

	/**
	 * Retrieve the amount of results for this page
	 * @result Integer value with the result count
	 **/
	int32 GetResultCount() const
	{
		return ResultCount;
	}

protected:
	/**
	 * @brief The index of this page
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult|Page")
	int32 PageIndex {};

	/**
	 * @brief The amount of results allowed to be displayed within a page
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult|Page")
	int32 PageSize {};

	/**
	 * @brief The total amount of pages
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult|Page")
	int32 PageCount {};

	/**
	 * @brief The total amount of results
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult")
	int32 TotalResultCount {};

	/**
	 * @brief The amount of results for this page
	 **/
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|PagedResult")
	int32 ResultCount {};
};
