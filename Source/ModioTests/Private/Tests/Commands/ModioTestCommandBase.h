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
#include "ModioErrorCondition.h"
#include "ModioSubsystem.h"
#include "Types/ModioErrorCode.h"

#if WITH_DEV_AUTOMATION_TESTS

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
	virtual bool CheckError(FModioErrorCode ec, EModioErrorCondition Value);

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
	EModioErrorCondition ExpectedResult;
	virtual bool CheckExpected(FModioErrorCode ec);

public:
	FModioTestLatentCommandBaseExpectedResult(FAutomationTestBase* AssociatedTest, EModioErrorCondition ExpectedResult)
		: FModioTestLatentCommandBase(AssociatedTest),
		  ExpectedResult(ExpectedResult) {};
	virtual ~FModioTestLatentCommandBaseExpectedResult() {}
};

#endif // WITH_DEV_AUTOMATION_TESTS