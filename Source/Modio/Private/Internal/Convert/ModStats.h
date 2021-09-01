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
#include "ModioSDK.h"
#include "Types/ModioModStats.h"

FORCEINLINE FModioModStats ToUnreal(const Modio::ModStats& In)
{
	FModioModStats Out;
	Out.PopularityRankPosition = ToUnreal(In.PopularityRankPosition);
	Out.PopularityRankTotalMods = ToUnreal(In.PopularityRankTotalMods);
	Out.DownloadsTotal = ToUnreal(In.DownloadsTotal);
	Out.SubscribersTotal = ToUnreal(In.SubscribersTotal);
	Out.RatingTotal = ToUnreal(In.RatingTotal);
	Out.RatingPositive = ToUnreal(In.RatingPositive);
	Out.RatingNegative = ToUnreal(In.RatingNegative);
	Out.RatingPercentagePositive = ToUnreal(In.RatingPercentagePositive);
	Out.RatingWeightedAggregate = ToUnreal(In.RatingWeightedAggregate);
	Out.RatingDisplayText = ToUnreal(In.RatingDisplayText);
	return Out;
}
