/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io SDK.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-sdk/blob/main/LICENSE>)
 *  
 */

#ifdef MODIO_SEPARATE_COMPILATION
	#include "modio/core/entities/ModioModStats.h"
#endif

#include "modio/detail/ModioJsonHelpers.h"

namespace Modio
{
	void from_json(const nlohmann::json& Json, Modio::ModStats& ModStats)
	{
		Detail::ParseSafe(Json, ModStats.PopularityRankPosition, "popularity_rank_position");
		Detail::ParseSafe(Json, ModStats.PopularityRankTotalMods, "popularity_rank_total_mods");
		Detail::ParseSafe(Json, ModStats.DownloadsTotal, "downloads_total");
		Detail::ParseSafe(Json, ModStats.SubscribersTotal, "subscribers_total");
		Detail::ParseSafe(Json, ModStats.RatingTotal, "ratings_total");
		Detail::ParseSafe(Json, ModStats.RatingPositive, "ratings_positive");
		Detail::ParseSafe(Json, ModStats.RatingNegative, "ratings_negative");
		Detail::ParseSafe(Json, ModStats.RatingPercentagePositive, "ratings_percentage_positive");
		Detail::ParseSafe(Json, ModStats.RatingWeightedAggregate, "ratings_weighted_aggregate");
		Detail::ParseSafe(Json, ModStats.RatingDisplayText, "ratings_display_text");
	}

	void to_json(nlohmann::json& Json, const Modio::ModStats Stats)
	{
		Json = nlohmann::json {{"popularity_rank_position", Stats.PopularityRankPosition},
							   {"popularity_rank_total_mods", Stats.PopularityRankTotalMods},
							   {"downloads_total", Stats.DownloadsTotal},
							   {"subscribers_total", Stats.SubscribersTotal},
							   {"ratings_total", Stats.RatingTotal},
							   {"ratings_positive", Stats.RatingPositive},
							   {"ratings_negative", Stats.RatingNegative},
							   {"ratings_percentage_positive", Stats.RatingPercentagePositive},
							   {"ratings_weighted_aggregate", Stats.RatingWeightedAggregate},
							   {"ratings_display_text", Stats.RatingDisplayText}};
	}

}

