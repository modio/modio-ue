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
class FModioArchiveModAsyncCommand : public FModioTestLatentCommandBaseExpectedResult
{
	FModioModID ModID;

public:
	FModioArchiveModAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID,
									 EModioErrorCondition ExpectedResult)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult),
		  ModID(ModID)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->ArchiveModAsync(ModID, FOnErrorOnlyDelegateFast::CreateSP(this, &FModioArchiveModAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("ArchiveMod completes without errors", ec, false);
		
        Done();
	}
};

#endif