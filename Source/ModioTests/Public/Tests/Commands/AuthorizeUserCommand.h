#pragma once
#include "Tests/Commands/ModioTestCommandBase.h"
#include "Types/ModioCommonTypes.h"

#if WITH_DEV_AUTOMATION_TESTS

class MODIOTESTS_API FModioAuthorizeUserCommand : public FModioTestLatentCommandBase
{
public:
	FModioAuthorizeUserCommand(FAutomationTestBase* InTestInstance) : FModioTestLatentCommandBase(InTestInstance) 
	{
		TimeoutSeconds = 15.0f;
	}

	virtual bool ModioUpdate(float DeltaTime) override;
	virtual void Start() override;

	bool CheckAuthCompletion();

	bool bAuthCheckInFlight = false;
	bool bAuthCompleted = false;
};

#endif