/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#pragma once
#include "ModioSubsystem.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioSubscribeToModAsyncCommand : public FModioTestLatentCommandBaseExpectedResult
{
	FModioModID ModID;

public:
	FModioSubscribeToModAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID,
									 EModioErrorCondition ExpectedResult)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult),
		  ModID(ModID)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->SubscribeToModAsync(
			ModID, FOnErrorOnlyDelegateFast::CreateSP(this, &FModioSubscribeToModAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("Subscription completes with expected result",
							   CheckExpected(ec),
							   true);
		Done();
	}
};
#endif