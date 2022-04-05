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

#include "Internal/ModioConvert.h"
#include "Types/ModioList.h"
#include "ModioSDK.h"

/** Convert a Modio::List to a FModioList */
template<template<typename...> class DestListType, typename DestValueType, typename SourceListType>
FModioList<DestListType, DestValueType> ToUnrealList(SourceListType Value)
{
	FModioList<DestListType, DestValueType> Result;
	Result.InternalList = ToUnreal<DestValueType>(Value.GetRawList());
	return Result;
}
