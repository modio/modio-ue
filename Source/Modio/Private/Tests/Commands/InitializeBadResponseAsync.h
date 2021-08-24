#pragma once

#include "ModioSubsystem.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioInitializeBadResponseAsyncCommand : public FModioTestLatentCommandBase
{
public:
	using FModioTestLatentCommandBase::FModioTestLatentCommandBase;
	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		FModioInitializeOptions Opts;
		Opts.APIKey = FModioApiKey("8d561adc585f3292356a20ef6502ae64");
		Opts.GameEnvironment = EModioEnvironment::Test;
		Opts.GameID = FModioGameID(788);

		Modio->InitializeAsync(Opts, FOnErrorOnlyDelegateFast::CreateSP(this, &FModioInitializeBadResponseAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("SDK initializes completes without errors", ec, false);
		Done();
	}
};
#endif