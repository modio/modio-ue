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
#include "Tests/Commands/ShutdownAsync.h"

#if WITH_DEV_AUTOMATION_TESTS


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioInitShutdownTest, "Modio.InitializationAndShutdown",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioInitShutdownTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));
	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}
#endif // WITH_DEV_AUTOMATION_TESTS
