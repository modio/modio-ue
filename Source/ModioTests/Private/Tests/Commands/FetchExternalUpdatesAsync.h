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
class FModioFetchExternalUpdatesAsyncCommand : public FModioTestLatentCommandBaseExpectedResult
{
public:
	FModioFetchExternalUpdatesAsyncCommand(FAutomationTestBase* AssociatedTest, EModioErrorCondition DesiredResult)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, DesiredResult)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->FetchExternalUpdatesAsync(FOnErrorOnlyDelegateFast::CreateSP(this, &FModioFetchExternalUpdatesAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("External updates fetched with expected result", CheckExpected(ec), true);
		Done();
	}
};
#endif