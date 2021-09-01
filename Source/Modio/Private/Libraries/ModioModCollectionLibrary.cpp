/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "Libraries/ModioModCollectionLibrary.h"

EModioModState UModioModCollectionLibrary::GetModState(const FModioModCollectionEntry& Entry)
{
	return Entry.GetModState();
}

FModioModID UModioModCollectionLibrary::GetID(const FModioModCollectionEntry& Entry)
{
	return Entry.GetID();
}

const FModioModInfo& UModioModCollectionLibrary::GetModProfile(const FModioModCollectionEntry& Entry)
{
	return Entry.GetModProfile();
}

const FString UModioModCollectionLibrary::GetPath(const FModioModCollectionEntry& Entry)
{
	return Entry.GetPath();
}
