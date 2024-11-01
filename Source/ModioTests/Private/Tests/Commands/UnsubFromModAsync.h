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
class FModioUnsubFromModAsyncCommand : public FModioTestLatentCommandBaseExpectedResult
{
FModioModID ModID;
public:
	FModioUnsubFromModAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID, EModioErrorCondition DesiredResult)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, DesiredResult),
		  ModID(ModID)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->UnsubscribeFromModAsync(ModID,
							   FOnErrorOnlyDelegateFast::CreateSP(this, &FModioUnsubFromModAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("Unsubscription completed with expected result", CheckExpected(ec), true);
		Done();
	}
};

#endif