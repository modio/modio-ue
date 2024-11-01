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
class FModioGetMutedUsersAsyncCommand : public FModioTestLatentCommandBaseExpectedResult
{


public:
	FModioGetMutedUsersAsyncCommand(FAutomationTestBase* AssociatedTest,
									 EModioErrorCondition ExpectedResult)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->GetMutedUsersAsync(FOnMuteUsersDelegateFast::CreateSP(this, &FModioGetMutedUsersAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec, TOptional<FModioUserList> MutedUsers)
	{
		CurrentTest->TestEqual("GetMutedUsers completes without errors", CheckExpected(ec), true);
		CurrentTest->TestTrue("When GetMutedUsers returns without error, MutedUsers should have a value",
							  MutedUsers.IsSet());
		Done();
	}
};


#endif