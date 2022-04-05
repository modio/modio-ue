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

#include "Templates/UnrealTemplate.h"

#if CPP
/**
 * Baseclass for lists that are returned from the API so that you can use the list transparantly from it's underlying
 * type and append on additional functionaly in subclasses
 */
template<template<typename...> class ContainerType, typename ValueType>
struct MODIO_API FModioList
{
	using ListType = ContainerType<ValueType>;

	using Iterator = typename ListType::TIterator;
	using ConstIterator = typename ListType::TConstIterator;

	FModioList() = default;
	FModioList(FModioList<ContainerType, ValueType>&& Other) : InternalList(MoveTemp(Other.InternalList)) {}
	FModioList(const FModioList& Other) : InternalList(Other.InternalList) {}
	FModioList(const ListType& FromList) : InternalList(FromList) {}

	FModioList& operator=(const FModioList& ModioList)
	{
		InternalList = ModioList.InternalList;
		return *this;
	}
	FModioList& operator=(FModioList&& ModioList)
	{
		InternalList = MoveTempIfPossible(ModioList.InternalList);
		return *this;
	}

	/// <summary>
	/// Fastest lookup of a index, will crash if the index is out of range
	/// </summary>
	/// <param name="Index"></param>
	/// <returns></returns>
	const ValueType& operator[](int Index) const
	{
		check(Index >= 0);
		check(Index < InternalList.size());

		return InternalList[Index];
	}

	/// <summary>
	///
	/// </summary>
	/// <param name="Index"></param>
	/// <returns>a nullptr if the value is out of range, else the element of the kind in the list</returns>
	const ValueType* At(int Index) const
	{
		if (Index < 0 && Index >= InternalList.size())
		{
			return &InternalList[Index];
		}
		return nullptr;
	}

	const ListType& GetRawList() const
	{
		return InternalList;
	}

	int32 Num() const
	{
		return InternalList.Num();
	}

	Iterator begin()
	{
		return InternalList.begin();
	}

	ConstIterator begin() const
	{
		return InternalList.begin();
	}

	Iterator end()
	{
		return InternalList.end();
	}

	ConstIterator end() const
	{
		return InternalList.end();
	}

	template<template<typename...> class DestListType, typename DestValueType, typename SourceListType>
	friend FModioList<DestListType, DestValueType> ToUnrealList(SourceListType Value);

	ListType InternalList;
};
#endif

