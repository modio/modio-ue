#include "Types/ModioModStats.h"
#include "ModioSDK.h"
#include "Internal/ModioConvert.h"

FModioModStats::FModioModStats(const Modio::ModStats& Stats)
	: PopularityRankPosition(ToUnreal(Stats.PopularityRankPosition)),
	  PopularityRankTotalMods(ToUnreal(Stats.PopularityRankTotalMods)),
	  DownloadsTotal(ToUnreal(Stats.DownloadsTotal)),
	  SubscribersTotal(ToUnreal(Stats.SubscribersTotal)),
	  RatingTotal(ToUnreal(Stats.RatingTotal)),
	  RatingPositive(ToUnreal(Stats.RatingPositive)),
	  RatingNegative(ToUnreal(Stats.RatingNegative)),
	  RatingPercentagePositive(ToUnreal(Stats.RatingPercentagePositive)),
	  RatingWeightedAggregate(ToUnreal(Stats.RatingWeightedAggregate)),
	  RatingDisplayText(ToUnreal(Stats.RatingDisplayText))
{}
