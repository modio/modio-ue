#pragma once
#include "ModioSubsystem.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioSubscribeToModAsyncCommand : public FModioTestLatentCommandBaseExpectedResult
{
	FModioModID ModID;

public:
	FModioSubscribeToModAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID,
									 ModioTestExpectedResult ExpectedResult)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult),
		  ModID(ModID)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->SubscribeToModAsync(
			ModID, FOnErrorOnlyDelegateFast::CreateSP(this, &FModioSubscribeToModAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("Subscription completes with expected result",
							   ExpectedResult.IsType<FModioErrorCode>()
								   ? ec.GetRawErrorCode() == ExpectedResult.Get<FModioErrorCode>().GetRawErrorCode()
								   : Modio::ErrorCodeMatches(ec.GetRawErrorCode(), ExpectedResult.Get<Modio::ErrorConditionTypes>()),
							   true);
		Done();
	}
};
#endif