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
#include "Types/ModioFilterParams.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioListAllModsAsyncCommand : public FModioTestLatentCommandBase
{
	FModioFilterParams Params;

public:
	using FModioTestLatentCommandBase::Update;

	// We may want to add a parameter here to retrieve the results so that the enclosing test can perform additional
	// work on the results
	FModioListAllModsAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioFilterParams& Params)
		: FModioTestLatentCommandBase(AssociatedTest),
		  Params(Params)
	{}

	virtual void Start() override
	{
		Modio->ListAllModsAsync(Params,
								FOnListAllModsDelegateFast::CreateSP(this, &FModioListAllModsAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec, TOptional<FModioModInfoList> ModInfoList)
	{
		CurrentTest->TestEqual("ListAllMods completes without errors", ec, false);
		CurrentTest->TestTrue("When ListAllMods returns without error, ModInfoList should have a value", ModInfoList.IsSet());
		Done();
	}
};

class FModioListAllModsBadResponseAsyncCommand : public FModioTestLatentCommandBase
{
	FModioFilterParams Params;

public:
	using FModioTestLatentCommandBase::Update;

	// We may want to add a parameter here to retrieve the results so that the enclosing test can perform additional
	// work on the results
	FModioListAllModsBadResponseAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioFilterParams& Params)
		: FModioTestLatentCommandBase(AssociatedTest),
		  Params(Params)
	{}

	virtual void Start() override
	{
		Modio->ListAllModsAsync(Params,
								FOnListAllModsDelegateFast::CreateSP(this, &FModioListAllModsBadResponseAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec, TOptional<FModioModInfoList> ModInfoList)
	{
		CurrentTest->TestEqual("ListAllMods completes with NetworkError error", CheckError(ec, EModioErrorCondition::NetworkError), true);
		CurrentTest->TestFalse("When ListAllMods returns with InvalidResponse error, ModInfoList should not have a value",
							  ModInfoList.IsSet());
		Done();
	}
};


#endif
