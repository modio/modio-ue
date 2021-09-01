/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS

ModioTestExpectedResult MakeExpected(Modio::ErrorCode RawErrorCode)
{
	ModioTestExpectedResult Expected;
	Expected.Set<FModioErrorCode>(FModioErrorCode(RawErrorCode));
	return Expected;
}

ModioTestExpectedResult MakeExpected(Modio::ErrorConditionTypes Condition)
{
	ModioTestExpectedResult Expected;
	Expected.Set<Modio::ErrorConditionTypes>(Condition);
	return Expected;
}

bool CheckExpectedValue(FModioErrorCode ec, ModioTestExpectedResult ExpectedResult)
{
	if (ExpectedResult.IsType<FModioErrorCode>())
	{
		return ec.GetRawErrorCode() == ExpectedResult.Get<FModioErrorCode>().GetRawErrorCode();
	}
	else
	{
		return Modio::ErrorCodeMatches(ec.GetRawErrorCode(), ExpectedResult.Get<Modio::ErrorConditionTypes>());
	}
}

 FModioTestLatentCommandBase::FModioTestLatentCommandBase(FAutomationTestBase* AssociatedTest)
	: CurrentTest(AssociatedTest)
{
	Modio = GEngine->GetEngineSubsystem<UModioSubsystem>();
}

bool FModioTestLatentCommandBase::Update()
{
	switch (CurrentState)
	{
		case LatentCommandState::NotStarted:
			CurrentState = LatentCommandState::InProgress;
			Start();
			return false;
			break;
		case LatentCommandState::InProgress:
			if (Modio == nullptr)
			{
				return true;
			}
			else
			{
				Modio->RunPendingHandlers();
				return false;
			}
			break;
		case LatentCommandState::Complete:

			return true;
			break;
	}
	return true;
}

void FModioTestLatentCommandBase::Done()
{
	CurrentState = LatentCommandState::Complete;
}

#endif
