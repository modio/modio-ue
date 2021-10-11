/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "Engine.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Misc/AutomationTest.h"
#include "Internal/Convert/ModStats.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioConvertDataToUnrealFormatTest, "Modio.DataConversion.NativeToUnreal.ModStats",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioConvertDataToUnrealFormatTest::RunTest(const FString& Parameters)
{
	Modio::ModStats InObj = {1, 2, 3, 4, 5, 6, 7, 8, 0.5f, "Positive"};
	FModioModStats OutObj = ToUnreal(InObj);
	TestEqual("PopularityRankPosition", OutObj.PopularityRankPosition, InObj.PopularityRankPosition);
	TestEqual("PopularityRankTotalMods", OutObj.PopularityRankTotalMods, InObj.PopularityRankTotalMods);
	TestEqual("DownloadsTotal", OutObj.DownloadsTotal, InObj.DownloadsTotal);
	TestEqual("SubscribersTotal", OutObj.SubscribersTotal, InObj.SubscribersTotal);
	TestEqual("RatingTotal", OutObj.RatingTotal, InObj.RatingTotal);
	TestEqual("RatingPositive", OutObj.RatingPositive, InObj.RatingPositive);
	TestEqual("RatingNegative", OutObj.RatingNegative, InObj.RatingNegative);
	TestEqual("RatingPercentagePositive", OutObj.RatingPercentagePositive, InObj.RatingPercentagePositive);
	TestEqual("RatingWeightedAggregate",
			  FMath::IsNearlyEqual((float) OutObj.RatingWeightedAggregate, (float) InObj.RatingWeightedAggregate),
			  true);
	TestEqual("RatingDisplayText", OutObj.RatingDisplayText.Equals(FString(InObj.RatingDisplayText.c_str())), true);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS