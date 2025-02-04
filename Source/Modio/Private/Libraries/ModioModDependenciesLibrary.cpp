/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioModDependenciesLibrary.h"

const TArray<FModioModDependency>& UModioModDependenciesLibrary::GetDependencies(
	const FModioModDependencyList& DependencyList)
{
	return DependencyList.InternalList;
}

const FModioPagedResult& UModioModDependenciesLibrary::GetPagedResult(const FModioModDependencyList& DependencyList)
{
	return DependencyList.PagedResult;
}

FModioUnsigned64 UModioModDependenciesLibrary::GetTotalFilesize(const FModioModDependencyList& DependencyList)
{
	return DependencyList.TotalFilesize;
}

FModioUnsigned64 UModioModDependenciesLibrary::GetTotalFilesizeUncompressed(
	const FModioModDependencyList& DependencyList)
{
	return DependencyList.TotalFilesizeUncompressed;
}