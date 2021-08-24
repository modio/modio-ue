#pragma once
#include "ModioSubsystem.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioUnsubFromModAsyncCommand : public FModioTestLatentCommandBase
{
FModioModID ModID;
bool bDesiredResult;
public:
	FModioUnsubFromModAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID, bool DesiredResult)
		: FModioTestLatentCommandBase(AssociatedTest),
		  ModID(ModID),
		  bDesiredResult(DesiredResult)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->UnsubscribeFromModAsync(ModID,
							   FOnErrorOnlyDelegateFast::CreateSP(this, &FModioUnsubFromModAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("Unsubscription completed with expected result", ec, bDesiredResult);
		Done();
	}
};

#endif