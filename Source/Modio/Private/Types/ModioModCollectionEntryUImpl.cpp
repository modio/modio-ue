/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Internal/ModioConvert.h"
#include "Types/ModioModCollectionEntry.h"

EModioModState FModioModCollectionEntry::GetModState() const
{
	return ModState;
}

FModioModID FModioModCollectionEntry::GetID() const
{
	return ModID;
}

const FModioModInfo& FModioModCollectionEntry::GetModProfile() const
{
	return ModProfile;
}

const FString FModioModCollectionEntry::GetPath() const
{
	if (ModPath)
	{
		return ModPath.GetValue();
	}
	else
	{
		return {};
	}
}

FModioUnsigned64 FModioModCollectionEntry::GetSizeOnDisk() const
{
	return SizeOnDisk;
}

FModioErrorCode FModioModCollectionEntry::GetLastError()
{
	return LastError;
}