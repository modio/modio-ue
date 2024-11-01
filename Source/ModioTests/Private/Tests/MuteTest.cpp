/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Engine/Engine.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Misc/AutomationTest.h"
#include "ModioSubsystem.h"
#include "ModioTestsFunctionLibrary.h"
#include "Tests/Commands/EnableModManagement.h"
#include "Tests/Commands/GetMutedUsersAsync.h"
#include "Tests/Commands/InitializeAsync.h"
#include "Tests/Commands/ShutdownAsync.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioMuteUserTest, "Modio.MuteUser.GetMutedUsers",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioMuteUserTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));
	ADD_LATENT_AUTOMATION_COMMAND(FModioGetMutedUsersAsyncCommand(this, EModioErrorCondition::NoError));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
