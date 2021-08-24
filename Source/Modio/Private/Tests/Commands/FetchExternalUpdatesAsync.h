#pragma once
#include "ModioSubsystem.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioFetchExternalUpdatesAsyncCommand : public FModioTestLatentCommandBase
{
	bool bDesiredResult;
public:
	FModioFetchExternalUpdatesAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID, bool DesiredResult)
		: FModioTestLatentCommandBase(AssociatedTest),
		  ModID(ModID),
		  bDesiredResult(DesiredResult)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->FetchExternalUpdatesAsync(FOnErrorOnlyDelegateFast::CreateSP(this, &FModioFetchExternalUpdatesAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("External updates fetched with expected result", ec, bDesiredResult);
		Done();
	}
};
#endif