/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
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
