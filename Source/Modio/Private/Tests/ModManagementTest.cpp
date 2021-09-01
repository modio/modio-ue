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
#include "Tests/Commands/DisableModManagement.h"
#include "Tests/Commands/EnableModManagement.h"
#include "Tests/Commands/InitializeAsync.h"
#include "Tests/Commands/IsModManagementBusy.h"
#include "Tests/Commands/QueryCurrentModUpdate.h"
#include "Tests/Commands/ShutdownAsync.h"
#include "Types/ModioFilterParams.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioModManagementEnableTest, "Modio.ModManagement.EnableDisable",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioModManagementEnableTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioEnableModManagementCommand(
		this, FOnModManagementDelegateFast::CreateLambda([](FModioModManagementEvent Event) {

		})));

	ADD_LATENT_AUTOMATION_COMMAND(FModioDisableModManagementCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioModManagementBusyManagementEnabledTest,
								 "Modio.ModManagement.Busy.ManagementEnabled",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioModManagementBusyManagementEnabledTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioEnableModManagementCommand(
		this, FOnModManagementDelegateFast::CreateLambda([](FModioModManagementEvent Event) {

		})));

	// Mod Management enabled, but no work = not busy
	ADD_LATENT_AUTOMATION_COMMAND(FModioIsModManagementBusyCommand(this, false));

	ADD_LATENT_AUTOMATION_COMMAND(FModioDisableModManagementCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioModManagementBusyManagementDisabledTest,
								 "Modio.ModManagement.Busy.ManagementDisabled",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioModManagementBusyManagementDisabledTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	// Mod Management disabled = not busy
	ADD_LATENT_AUTOMATION_COMMAND(FModioIsModManagementBusyCommand(this, false));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioModManagementQueryCurrentUpdateManagementDisabled, "Modio.ModManagement.QueryCurrentUpdate.ManagementDisabled",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioModManagementQueryCurrentUpdateManagementDisabled::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	TOptional<FModioModProgressInfo> ProgressInfo;
	ADD_LATENT_AUTOMATION_COMMAND(FModioQueryCurrentUpdateCommand(this, ProgressInfo));

	TestEqual("QueryCurrentUpdate returns empty optional when mod management is disabled", ProgressInfo.IsSet(), false);

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));

	return true;
}



IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioModManagementQueryCurrentUpdateNoWork,
								 "Modio.ModManagement.QueryCurrentUpdate.NoUpdate",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioModManagementQueryCurrentUpdateNoWork::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioEnableModManagementCommand(
		this, FOnModManagementDelegateFast::CreateLambda([](FModioModManagementEvent Event) {

		})));

	TOptional<FModioModProgressInfo> ProgressInfo;
	ADD_LATENT_AUTOMATION_COMMAND(FModioQueryCurrentUpdateCommand(this, ProgressInfo));

	TestEqual("QueryCurrentUpdate returns empty optional when no update occurring", ProgressInfo.IsSet(), false);

	ADD_LATENT_AUTOMATION_COMMAND(FModioDisableModManagementCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));

	return true;
}
#endif