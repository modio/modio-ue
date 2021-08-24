#pragma once
#include "ModioSubsystem.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioIsModManagementBusyCommand : public FModioTestLatentCommandBase
{
	bool Result;
public:
	FModioIsModManagementBusyCommand(FAutomationTestBase* AssociatedTest, bool DesiredResult)
		: FModioTestLatentCommandBase(AssociatedTest),
		  Result(DesiredResult){};

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		CurrentTest->TestEqual("IsModManagementBusy returns expected result", Result, Modio->IsModManagementBusy());
		Done();
	}
};
#endif