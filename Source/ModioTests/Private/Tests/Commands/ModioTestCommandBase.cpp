/* 
 *  Copyright (C) 2024-2026 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "Tests/Commands/ModioTestCommandBase.h"
#include "Libraries/ModioErrorConditionLibrary.h"
#include "ModioTestsModule.h"

#if WITH_DEV_AUTOMATION_TESTS

bool FModioTestLatentCommandBase::CheckError(FModioErrorCode ec, EModioErrorCondition Value) 
{
	return UModioErrorConditionLibrary::ErrorCodeMatches(ec, Value);
}

FModioTestLatentCommandBase::FModioTestLatentCommandBase(FAutomationTestBase* AssociatedTest)
	: CurrentTest(AssociatedTest)
{
	Modio = GEngine->GetEngineSubsystem<UModioSubsystem>();
}

void FModioTestLatentCommandBase::AddInfo(const FString& InInfo) 
{
	CurrentTest->AddInfo(InInfo);
	UE_LOG(LogModioTests, Log, TEXT("AUTOMATION: %s"), *InInfo);
}

void FModioTestLatentCommandBase::AddError(const FString& InError) 
{
	CurrentTest->AddError(InError);
	UE_LOG(LogModioTests, Error, TEXT("AUTOMATION: %s"), *InError);
	Done();
}

bool FModioTestLatentCommandBase::Update()
{
	double CurrentTime = FPlatformTime::Seconds();
	float DeltaTime = static_cast<float>(CurrentTime - LastUpdateTime);
	RepeatedLogTimer -= DeltaTime;

	switch (CurrentState)
	{
		case LatentCommandState::NotStarted:
			CurrentState = LatentCommandState::InProgress;
			LastUpdateTime = FPlatformTime::Seconds();
			Start();
			return false;
		case LatentCommandState::InProgress:
			LastUpdateTime = CurrentTime;
			if (ModioUpdate(DeltaTime))
			{
				Done();
				break;
			}

			if (TimeoutSeconds > 0.0f)
			{
				TimeoutSeconds -= DeltaTime;
				if (TimeoutSeconds <= 0.0f)
				{
					AddError(TEXT("Command timed out"));
					break;
				}
			}

			if (Modio == nullptr)
			{
				return true;
			}
			else
			{
				if (!Modio->IsUsingBackgroundThread())
				{
					Modio->RunPendingHandlers();
				}

				return false;
			}
			break;
		case LatentCommandState::Complete:
			return true;
	}
	return true;
}

void FModioTestLatentCommandBase::Done()
{
	CurrentState = LatentCommandState::Complete;
}

bool FModioTestLatentCommandBaseExpectedResult::CheckExpected(FModioErrorCode ec) 
{
	return CheckError(ec, ExpectedResult);
}

#endif