/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

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
	// In linux, TestEqual can not figure out the conversion between int32 and int64, for that reason, it is explicit here.
	TestEqual("PopularityRankPosition", (int32)OutObj.PopularityRankPosition, (int32)InObj.PopularityRankPosition);
	TestEqual("PopularityRankTotalMods", (int32)OutObj.PopularityRankTotalMods, (int32)InObj.PopularityRankTotalMods);
	TestEqual("DownloadsTotal", (int32)OutObj.DownloadsTotal, (int32)InObj.DownloadsTotal);
	TestEqual("SubscribersTotal", (int32)OutObj.SubscribersTotal, InObj.SubscribersTotal);
	TestEqual("RatingTotal", (int32)OutObj.RatingTotal, (int32)InObj.RatingTotal);
	TestEqual("RatingPositive", (int32)OutObj.RatingPositive, (int32)InObj.RatingPositive);
	TestEqual("RatingNegative", (int32)OutObj.RatingNegative, (int32)InObj.RatingNegative);
	TestEqual("RatingPercentagePositive", (int32)OutObj.RatingPercentagePositive, (int32)InObj.RatingPercentagePositive);
	TestEqual("RatingWeightedAggregate", FMath::IsNearlyEqual((float) OutObj.RatingWeightedAggregate, (float) InObj.RatingWeightedAggregate),
			  true);
	TestEqual(TEXT("RatingDisplayText"), OutObj.RatingDisplayText.Equals(FString(InObj.RatingDisplayText.c_str())), true);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS