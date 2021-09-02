/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "Libraries/ModioModDependenciesLibrary.h"

const TArray<FModioModDependency>& UModioModDependenciesLibrary::GetDependencies(const FModioModDependencyList& DependencyList)
{
	return DependencyList.InternalList;
}

const FModioPagedResult& UModioModDependenciesLibrary::GetPagedResult(
	const FModioModDependencyList& DependencyList)
{
	return DependencyList.PagedResult;
}
