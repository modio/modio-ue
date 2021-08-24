#pragma once
#include "ModioSubsystem.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioQueryCurrentUpdateCommand : public FModioTestLatentCommandBase
{
TOptional<FModioModProgressInfo>& Results;
public:
	
	FModioQueryCurrentUpdateCommand(FAutomationTestBase* AssociatedTest, TOptional<FModioModProgressInfo>& Results)
		: FModioTestLatentCommandBase(AssociatedTest),
		  Results(Results) {};

	using FModioTestLatentCommandBase::Update;

	virtual void Start() override
	{
		Results = Modio->QueryCurrentModUpdate();
		Done();
	}
};
#endif