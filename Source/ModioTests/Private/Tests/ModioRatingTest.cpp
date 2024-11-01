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
#include "Tests/Commands/InitializeAsync.h"
#include "Tests/Commands/ShutdownAsync.h"
#include "Tests/Commands/SubmitRatingAsync.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioSubmitPositiveRatingTest, "Modio.Rating.Positive",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioSubmitPositiveRatingTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));
	ADD_LATENT_AUTOMATION_COMMAND(FModioSubmitRatingAsyncCommand(
		this, UModioTestsFunctionLibrary::GetTestModID(), EModioRating::Positive, EModioErrorCondition::NoError));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioSubmitNeutralRatingTest, "Modio.Rating.Neutral",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioSubmitNeutralRatingTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));
	ADD_LATENT_AUTOMATION_COMMAND(FModioSubmitRatingAsyncCommand(this, UModioTestsFunctionLibrary::GetTestModID(),
																 EModioRating::Neutral, EModioErrorCondition::NoError));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioSubmitNegativeRatingTest, "Modio.Rating.Negative",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioSubmitNegativeRatingTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));
	ADD_LATENT_AUTOMATION_COMMAND(FModioSubmitRatingAsyncCommand(
		this, UModioTestsFunctionLibrary::GetTestModID(), EModioRating::Negative, EModioErrorCondition::NoError));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
