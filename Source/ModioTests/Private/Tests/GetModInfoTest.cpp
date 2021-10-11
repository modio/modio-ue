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
#include "Tests/Commands/ShutdownAsync.h"
#include "Tests/Commands/GetModInfoAsync.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioGetModInfoTest, "Modio.GetModInfo",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioGetModInfoTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	//ADD_LATENT_AUTOMATION_COMMAND(FModioGetModInfoAsyncCommand(this, FModioModID(), FModioErrorCode {}));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioGetModInfoBadResponseTest, "Modio.GetModInfo.InvalidResponseHandled",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioGetModInfoBadResponseTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeBadResponseAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioGetModInfoBadResponseAsyncCommand(this, FModioModID(1), EModioErrorCondition::NetworkError));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
