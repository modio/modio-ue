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
#include "Tests/Commands/InitializeAsync.h"
#include "Tests/Commands/InitializeBadResponseAsync.h"
#include "Tests/Commands/ListAllModsAsync.h"
#include "Tests/Commands/ShutdownAsync.h"
#include "Types/ModioFilterParams.h"

#if WITH_DEV_AUTOMATION_TESTS



IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsInvalidResponseTest, "Modio.ListAllMods.InvalidResponseHandled",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsInvalidResponseTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeBadResponseAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsBadResponseAsyncCommand(this, FModioFilterParams()));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}



IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsNoFilterTest, "Modio.ListAllMods.NoFilter",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsNoFilterTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));
	
	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(this, FModioFilterParams()));
	
	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsSortDateLiveTest, "Modio.ListAllMods.Sorting.DateLive",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsSortDateLiveTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::DateMarkedLive, EModioSortDirection::Ascending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::DateMarkedLive, EModioSortDirection::Descending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsSortDateUpdatedTest, "Modio.ListAllMods.Sorting.DateUpdated",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsSortDateUpdatedTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::DateUpdated, EModioSortDirection::Ascending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::DateUpdated, EModioSortDirection::Descending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsSortDownloadsTest, "Modio.ListAllMods.Sorting.DownloadsToday",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsSortDownloadsTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::DownloadsToday, EModioSortDirection::Ascending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::DownloadsToday, EModioSortDirection::Descending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsSortIDTest, "Modio.ListAllMods.Sorting.ID",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsSortIDTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::ID, EModioSortDirection::Ascending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::ID, EModioSortDirection::Descending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsSortRatingTest, "Modio.ListAllMods.Sorting.Rating",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsSortRatingTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::Rating, EModioSortDirection::Ascending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::Rating, EModioSortDirection::Descending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsSortSubsTest, "Modio.ListAllMods.Sorting.SubCount",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsSortSubsTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::SubscriberCount, EModioSortDirection::Ascending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().SortBy(EModioSortFieldType::SubscriberCount, EModioSortDirection::Descending)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsNameContainsTest, "Modio.ListAllMods.NameSubstringSearch",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsNameContainsTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().NameContains("SomeString")));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsPagedTest, "Modio.ListAllMods.ResultRange.Paged",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsPagedTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(this, FModioFilterParams().PagedResults(0, 64)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsIndexedTest, "Modio.ListAllMods.ResultRange.Indexed",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsIndexedTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(
		this, FModioFilterParams().IndexedResults(3, 55)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsIDFilterTest, "Modio.ListAllMods.IncludeExcludeModIDs",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsIDFilterTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	TArray<FModioModID> ModList;
	ModList.Add(FModioModID(1));
	ModList.Add(FModioModID(2));
	ModList.Add(FModioModID(3));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(this, FModioFilterParams().MatchingIDs(ModList)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(this, FModioFilterParams().ExcludingIDs(ModList)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioListAllModsMetadataFilterTest, "Modio.ListAllMods.MetadataFilter",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioListAllModsMetadataFilterTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));


	ADD_LATENT_AUTOMATION_COMMAND(FModioListAllModsAsyncCommand(this, FModioFilterParams().MetadataLike("SomeString")));


	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
