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
#include "ModioTestsFunctionLibrary.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioInitializeAsyncCommand : public FModioTestLatentCommandBase
{
public:
	using FModioTestLatentCommandBase::FModioTestLatentCommandBase;
	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->InitializeAsync(UModioTestsFunctionLibrary::GetAutomationTestOptions(),
							   FOnErrorOnlyDelegateFast::CreateSP(this, &FModioInitializeAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("SDK initializes completes without errors", ec, false);
		Done();
	}
};
#endif