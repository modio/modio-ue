/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "Types/ModioModCollectionEntry.h"
#include "Internal/ModioConvert.h"

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
