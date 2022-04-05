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
#include "Types/ModioModCreationHandle.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioCreateModAsyncCommand : public FModioTestLatentCommandBase
{
	FModioModID* NewModID;
	FModioModCreationHandle CreationHandle;
	FModioCreateModParams Params;
public:
	using FModioTestLatentCommandBase::Update;

	FModioCreateModAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioCreateModParams& Params, FModioModID* const NewModID)
		: FModioTestLatentCommandBase(AssociatedTest),
		  NewModID(NewModID),
		Params(Params)
	{}

	virtual void Start() override
	{
		CreationHandle = Modio->GetModCreationHandle();
		Modio->SubmitNewModAsync(CreationHandle, Params, FOnSubmitNewModDelegateFast::CreateSP(this, &FModioCreateModAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec, TOptional<FModioModID> ModID)
	{
		CurrentTest->TestEqual("CreateMod completes without errors", ec, false);
		CurrentTest->TestTrue("When CreateModAsync returns without error, ModID should have a value", ModID.IsSet());
		if (ModID.IsSet())
		{
			*NewModID = ModID.GetValue();
		}
		Done();
	}
};
class FModioSubmitModFileAsyncCommand : public FModioTestLatentCommandBase
{
	FModioModID ModID;
	FModioCreateModFileParams Params;
	TSharedPtr<TOptional<FModioErrorCode>> SharedState;
public:
	

	//needs to take in some shared state with the mod management loop stuff
	FModioSubmitModFileAsyncCommand(FAutomationTestBase* AssociatedTest,
		FModioModID ModID, FModioCreateModFileParams Params, TSharedPtr<TOptional<FModioErrorCode>> SharedErrorCode)
		: FModioTestLatentCommandBase(AssociatedTest),
		ModID(ModID),
		Params(Params),
		SharedState(SharedErrorCode)
	{}

	virtual void Start() override
	{
		Modio->SubmitNewModFileForMod(ModID, Params);
	}
	virtual bool Update() override
	{
		CurrentTest->TestValid("Shared state should always be valid", SharedState);
		//The optional inside the shared state may not be set yet because the upload isn't complete
		if (SharedState->IsSet())
		{
			//If the optional inside the shared state is set, then it means that the upload is complete and we can check the error code itself
			CurrentTest->TestEqual("Submit Mod File should complete without errors", SharedState->GetValue(), false);
			Done();
		}
		return FModioTestLatentCommandBase::Update();
	}
};


#endif
