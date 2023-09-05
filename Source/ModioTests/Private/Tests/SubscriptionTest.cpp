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
#include "ModioSubsystem.h"
#include "ModioTestsFunctionLibrary.h"
#include "Tests/Commands/EnableModManagement.h"
#include "Tests/Commands/InitializeAsync.h"
#include "Tests/Commands/ShutdownAsync.h"
#include "Tests/Commands/SubscribeToModAsync.h"
#include "Tests/Commands/UnsubFromModAsync.h"
#include "Types/ModioModManagementEvent.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioAddNewSubTestNoInit, "Modio.Subscription.AddNewSubscription_PluginNotInitialized",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioAddNewSubTestNoInit::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioSubscribeToModAsyncCommand(this, UModioTestsFunctionLibrary::GetTestModID(),
																   EModioErrorCondition::SDKNotInitialized));

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioAddNewSubTest, "Modio.Subscription.AddNewSubscription",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioAddNewSubTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));

	ADD_LATENT_AUTOMATION_COMMAND(FModioEnableModManagementCommand(
		this, FOnModManagementDelegateFast::CreateLambda([](FModioModManagementEvent Event) {})));

	ADD_LATENT_AUTOMATION_COMMAND(FModioSubscribeToModAsyncCommand(this, UModioTestsFunctionLibrary::GetTestModID(),
																   EModioErrorCondition::NoError));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsubscribeTest, "Modio.Subscription.UnsubscribeFromMod",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioUnsubscribeTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));
	ADD_LATENT_AUTOMATION_COMMAND(FModioEnableModManagementCommand(
		this, FOnModManagementDelegateFast::CreateLambda([](FModioModManagementEvent Event) {})));

	ADD_LATENT_AUTOMATION_COMMAND(FModioUnsubFromModAsyncCommand(this, UModioTestsFunctionLibrary::GetTestModID(),
																 EModioErrorCondition::NoError));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioUnsubscribeTestNoInit, "Modio.Subscription.UnsubscribeFromMod_PluginNotInitialized",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioUnsubscribeTestNoInit::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioUnsubFromModAsyncCommand(this, UModioTestsFunctionLibrary::GetTestModID(),
																 EModioErrorCondition::SDKNotInitialized));

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioDuplicateSubTest, "Modio.Subscription.SubToAlreadySubscribedMod",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioDuplicateSubTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));
	ADD_LATENT_AUTOMATION_COMMAND(FModioEnableModManagementCommand(
		this, FOnModManagementDelegateFast::CreateLambda([](FModioModManagementEvent Event) {})));
	ADD_LATENT_AUTOMATION_COMMAND(FModioSubscribeToModAsyncCommand(this, UModioTestsFunctionLibrary::GetTestModID(),
																   EModioErrorCondition::NoError));

	ADD_LATENT_AUTOMATION_COMMAND(FModioSubscribeToModAsyncCommand(this, UModioTestsFunctionLibrary::GetTestModID(),
																   EModioErrorCondition::NoError));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
