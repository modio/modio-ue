/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Engine/Engine.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Misc/AutomationTest.h"
#include "Tests/Commands/CreateModAsync.h"
#include "Tests/Commands/EnableModManagement.h"
#include "Tests/Commands/InitializeAsync.h"
#include "Tests/Commands/ShutdownAsync.h"
#include "Types/ModioFilterParams.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioCreateModAndModFileTest, "ModioSpecialTests.SubmitNewModAndModfile",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioCreateModAndModFileTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	FModioModID NewModID;
	TSharedPtr<TOptional<FModioErrorCode>> SharedState = MakeShared<TOptional<FModioErrorCode>>();
	ADD_LATENT_AUTOMATION_COMMAND(FModioCreateModAsyncCommand(this, FModioCreateModParams{}, &NewModID));

	ADD_LATENT_AUTOMATION_COMMAND(FModioEnableModManagementCommand(this, FOnModManagementDelegateFast::CreateLambda([SharedState, NewModID](FModioModManagementEvent Event)
		{
			if (Event.ID == NewModID)
			{
				*SharedState = Event.Status;
			}
		}
	)));

	ADD_LATENT_AUTOMATION_COMMAND(FModioSubmitModFileAsyncCommand(this, NewModID, FModioCreateModFileParams{}, SharedState));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
