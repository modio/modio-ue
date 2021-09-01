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
class FModioUnsubFromModAsyncCommand : public FModioTestLatentCommandBase
{
FModioModID ModID;
bool bDesiredResult;
public:
	FModioUnsubFromModAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID, bool DesiredResult)
		: FModioTestLatentCommandBase(AssociatedTest),
		  ModID(ModID),
		  bDesiredResult(DesiredResult)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->UnsubscribeFromModAsync(ModID,
							   FOnErrorOnlyDelegateFast::CreateSP(this, &FModioUnsubFromModAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("Unsubscription completed with expected result", ec, bDesiredResult);
		Done();
	}
};

#endif