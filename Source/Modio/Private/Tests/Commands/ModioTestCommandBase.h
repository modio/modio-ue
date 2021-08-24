#pragma once

#include "Engine.h"
#include "Misc/AutomationTest.h"
#include "Misc/TVariant.h"
#include "ModioSubsystem.h"
#include "Types/ModioErrorCode.h"

#if WITH_DEV_AUTOMATION_TESTS

using ModioTestExpectedResult = TVariant<FModioErrorCode, Modio::ErrorConditionTypes>;

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
	FModioTestLatentCommandBase(FAutomationTestBase* AssociatedTest) : CurrentTest(AssociatedTest)
	{
		Modio = GEngine->GetEngineSubsystem<UModioSubsystem>();
	}
	virtual ~FModioTestLatentCommandBase() {}

	virtual bool Update() override
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
	UModioSubsystem* Modio = nullptr;
	virtual void Start() = 0;
	virtual void Done()
	{
		CurrentState = LatentCommandState::Complete;
	}
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