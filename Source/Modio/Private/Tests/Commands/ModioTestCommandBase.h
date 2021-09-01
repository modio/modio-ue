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

#include "Engine.h"
#include "Misc/AutomationTest.h"
#include "Misc/TVariant.h"
#include "ModioSubsystem.h"
#include "Types/ModioErrorCode.h"

#if WITH_DEV_AUTOMATION_TESTS

using ModioTestExpectedResult = TVariant<FModioErrorCode, Modio::ErrorConditionTypes>;

ModioTestExpectedResult MakeExpected(Modio::ErrorCode RawErrorCode);

ModioTestExpectedResult MakeExpected(Modio::ErrorConditionTypes Condition);

bool CheckExpectedValue(FModioErrorCode ec, ModioTestExpectedResult ExpectedResult);

class FModioTestLatentCommandBase : public IAutomationLatentCommand
{
	enum class LatentCommandState
	{
		NotStarted,
		InProgress,
		Complete
	};
	LatentCommandState CurrentState = LatentCommandState::NotStarted;

protected:
	FAutomationTestBase* CurrentTest = nullptr;

public:
	FModioTestLatentCommandBase(FAutomationTestBase* AssociatedTest);
	virtual ~FModioTestLatentCommandBase() {}

	virtual bool Update() override;
	UModioSubsystem* Modio = nullptr;
	virtual void Start() = 0;
	virtual void Done();
};

class FModioTestLatentCommandBaseExpectedResult : public FModioTestLatentCommandBase
{
protected:
	ModioTestExpectedResult ExpectedResult;

public:
	FModioTestLatentCommandBaseExpectedResult(FAutomationTestBase* AssociatedTest,
											  ModioTestExpectedResult ExpectedResult)
		: FModioTestLatentCommandBase(AssociatedTest),
		  ExpectedResult(ExpectedResult) {};
};

#endif // WITH_DEV_AUTOMATION_TESTS