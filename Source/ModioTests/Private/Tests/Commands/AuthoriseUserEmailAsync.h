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
class FModioAuthoriseUserEmailAsync : public FModioTestLatentCommandBaseExpectedResult
{
	FModioEmailAuthCode AuthCode;

public:
	FModioAuthoriseUserEmailAsync(FAutomationTestBase* AssociatedTest, FModioEmailAuthCode AuthCode,
								  ModioTestExpectedResult ExpectedResult)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult),
		  AuthCode(AuthCode)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		/*
			Modio->AuthenticateUserEmailAsync(Email, FOnErrorOnlyDelegateFast::CreateSP(this,
						   &FModioAuthoriseUserEmailAsync::Callback));
						*/
	}
	void Callback(FModioErrorCode ec)
	{
		/*
				CurrentTest->TestEqual(
					"Authentication completes with expected result",
					ExpectedResult.IsType<FModioErrorCode>()
						? ec.GetRawErrorCode() == ExpectedResult.Get<FModioErrorCode>().GetRawErrorCode()
						: Modio::ErrorCodeMatches(ec.GetRawErrorCode(),
		   ExpectedResult.Get<Modio::ErrorConditionTypes>()), true);
		*/
		Done();
	}
};
#endif