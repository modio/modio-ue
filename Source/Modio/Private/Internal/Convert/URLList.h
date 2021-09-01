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
#include "ModioSDK.h"
#include "Types/ModioURLList.h"

FORCEINLINE FModioYoutubeURLList ToUnreal(const Modio::YoutubeURLList& In)
{
	return FModioYoutubeURLList(In);
}


FORCEINLINE FModioSketchfabURLList ToUnreal(const Modio::SketchfabURLList& In)
{
	return FModioSketchfabURLList(In);
}