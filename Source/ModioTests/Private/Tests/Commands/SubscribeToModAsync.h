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
class FModioSubscribeToModAsyncCommand : public FModioTestLatentCommandBaseExpectedResult
{
	FModioModID ModID;
	bool IncludeDependencies;

public:
	FModioSubscribeToModAsyncCommand(FAutomationTestBase* AssociatedTest, FModioModID ModID, bool IncludeDependencies,
									 EModioErrorCondition ExpectedResult)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult),
		  ModID(ModID), IncludeDependencies(IncludeDependencies)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->SubscribeToModAsync(
			ModID, IncludeDependencies, FOnErrorOnlyDelegateFast::CreateSP(this, &FModioSubscribeToModAsyncCommand::Callback));
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