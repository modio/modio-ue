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
class FModioFetchExternalUpdatesAsyncCommand : public FModioTestLatentCommandBase
{
	bool bDesiredResult;
public:
	FModioFetchExternalUpdatesAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID, bool DesiredResult)
		: FModioTestLatentCommandBase(AssociatedTest),
		  ModID(ModID),
		  bDesiredResult(DesiredResult)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->FetchExternalUpdatesAsync(FOnErrorOnlyDelegateFast::CreateSP(this, &FModioFetchExternalUpdatesAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("External updates fetched with expected result", ec, bDesiredResult);
		Done();
	}
};
#endif