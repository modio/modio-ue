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
class FModioGetUserCreationsAsyncCommand : public FModioTestLatentCommandBaseExpectedResult
{
public:
	FModioGetUserCreationsAsyncCommand(FAutomationTestBase* AssociatedTest,
									 EModioErrorCondition ExpectedResult)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->ListUserCreatedModsAsync(FOnListUserCreatedModsDelegateFast::CreateSP(this, &FModioGetUserCreationsAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec, TOptional<FModioModInfoList> CreatedMods)
	{
		CurrentTest->TestEqual("ListUserCreatedMods completes without errors", CheckExpected(ec), true);
		CurrentTest->TestTrue("When ListUserCreatedMods returns without error, CreatedMods should have a value",
							  CreatedMods.IsSet());
		Done();
	}
};


#endif