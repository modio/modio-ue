/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

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
		Opts.ApiKey = FModioApiKey("8d561adc585f3292356a20ef6502ae64");
		Opts.GameEnvironment = EModioEnvironment::Test;
		Opts.GameId = FModioGameID(788);
		Opts.LocalSessionIdentifier = "test";
		Opts.bUseBackgroundThread = UModioSDKLibrary::GetProjectInitializeOptions().bUseBackgroundThread;

		Modio->InitializeAsync(Opts, FOnErrorOnlyDelegateFast::CreateSP(this, &FModioInitializeBadResponseAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("SDK initializes completes without errors", ec, false);
		Done();
	}
};
#endif