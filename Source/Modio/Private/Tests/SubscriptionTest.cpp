#include "Engine.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Misc/AutomationTest.h"
#include "Tests/Commands/InitializeAsync.h"
#include "Tests/Commands/SubscribeToModAsync.h"
#include "Tests/Commands/ShutdownAsync.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioAddNewSubTest, "Modio.Subscription.AddNewSubscription",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioAddNewSubTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));
	//will need to authenticate here?

	//ADD_LATENT_AUTOMATION_COMMAND(FModioSubscribeToModAsyncCommand(this, FModioModID(), FModioErrorCode {}));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModioDuplicateSubTest, "Modio.Subscription.SubToAlreadySubscribedMod",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext |
									 EAutomationTestFlags::ProductFilter)
bool FModioDuplicateSubTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FModioInitializeAsyncCommand(this));
	// will need to authenticate here?

	//ADD_LATENT_AUTOMATION_COMMAND(FModioSubscribeToModAsyncCommand(this, FModioModID(), FModioErrorCode {}));

	//ADD_LATENT_AUTOMATION_COMMAND(FModioSubscribeToModAsyncCommand(this, FModioModID(), FModioErrorCode(Modio::make_error_code(Modio::ModManagementError::AlreadySubscribed))));

	ADD_LATENT_AUTOMATION_COMMAND(FModioShutdownAsyncCommand(this));
	return true;
}



#endif // WITH_DEV_AUTOMATION_TESTS
