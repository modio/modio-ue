/*
 *  Copyright (C) 2024-2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Engine/Engine.h"
#include "Misc/AutomationTest.h"
#include "Misc/TVariant.h"
#include "ModioErrorCondition.h"
#include "ModioSubsystem.h"
#include "Types/ModioErrorCode.h"

#if WITH_DEV_AUTOMATION_TESTS

class MODIOTESTS_API FModioTestLatentCommandBase : public IAutomationLatentCommand
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

	double LastUpdateTime = 0.0;
	float RepeatedLogTimer = 0.0f;
	float TimeoutSeconds = -1.0f;

public:
	FModioTestLatentCommandBase(FAutomationTestBase* AssociatedTest);
	virtual ~FModioTestLatentCommandBase() {}

	virtual bool Update() override;
	virtual bool ModioUpdate(float DeltaTime) { return false; }
	UModioSubsystem* Modio = nullptr;
	virtual void Start() = 0;
	virtual void Done();

	void AddInfo(const FString& InInfo);
	void AddError(const FString& InError);
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