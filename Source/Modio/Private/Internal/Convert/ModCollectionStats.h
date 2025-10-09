/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioModCollection.h"

FORCEINLINE FModioModCollectionStats ToUnreal(const Modio::ModCollectionStats& In)
{
    FModioModCollectionStats Out;
    
    Out.DownloadsToday = ToUnreal(In.DownloadsToday);
	Out.DownloadsTotal = ToUnreal(In.DownloadsTotal);
	Out.DownloadsUnique = ToUnreal(In.DownloadsUnique);
	Out.RatingTotal30Days = ToUnreal(In.RatingTotal30Days);
	Out.RatingPositive30Days = ToUnreal(In.RatingPositive30Days);
	Out.RatingNegative30Days = ToUnreal(In.RatingNegative30Days);
	Out.RatingTotal = ToUnreal(In.RatingTotal);
	Out.RatingPositive = ToUnreal(In.RatingPositive);
	Out.RatingNegative = ToUnreal(In.RatingNegative);
	Out.NumberOfComments = ToUnreal(In.NumberOfComments);
	Out.NumberOfFollowers = ToUnreal(In.NumberOfFollowers);
	Out.NumberOfMods = ToUnreal(In.NumberOfMods);
    
    return Out;
}