#pragma once
#include "ModioSubsystem.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioInitializeAsyncCommand : public FModioTestLatentCommandBase
{
public:
	using FModioTestLatentCommandBase::FModioTestLatentCommandBase;
	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->InitializeAsync(UModioSDKLibrary::GetProjectInitializeOptions(),
							   FOnErrorOnlyDelegateFast::CreateSP(this, &FModioInitializeAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("SDK initializes completes without errors", ec, false);
		Done();
	}
};
#endif