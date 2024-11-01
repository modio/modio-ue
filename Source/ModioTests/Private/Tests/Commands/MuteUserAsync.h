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
class FModioMuteUserAsyncCommand : public FModioTestLatentCommandBaseExpectedResult
{
	FModioModID ModID;

public:
	FModioMuteUserAsyncCommand(FAutomationTestBase* AssociatedTest,
									 EModioErrorCondition ExpectedResult)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult),
		  ModID(ModID)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		//Modio->GetTermsOfUseAsync(EModioAuthenticationProvider::Steam,EModioLanguage::English, FOnGetTermsOfUseDelegateFast::CreateSP(this, &FModioMuteUserAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec, TOptional<FModioTerms> Terms)
	{
		/*CurrentTest->TestEqual("GetTermsOfUse completes without errors", CheckExpected(ec), true);
		CurrentTest->TestTrue("When GetTermsOfUse returns without error, Terms should have a value",
							  Terms.IsSet());*/
		Done();
	}
};


#endif