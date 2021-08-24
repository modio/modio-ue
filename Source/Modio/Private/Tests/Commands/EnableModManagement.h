#pragma once
#include "ModioSubsystem.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioEnableModManagementCommand : public FModioTestLatentCommandBase
{
FOnModManagementDelegateFast Callback;
public:
	FModioEnableModManagementCommand(FAutomationTestBase* AssociatedTest,  const FOnModManagementDelegateFast& Callback)
	:FModioTestLatentCommandBase(AssociatedTest),
	Callback(Callback){};

	using FModioTestLatentCommandBase::Update;
	
	virtual void Start() override
	{
		Modio->EnableModManagement(Callback);
		Done();
	}
	
};
#endif