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
class FModioSubmitRatingAsyncCommand : public FModioTestLatentCommandBaseExpectedResult
{
	FModioModID ModID;
    EModioRating Rating;

public:
	FModioSubmitRatingAsyncCommand(FAutomationTestBase* AssociatedTest, const FModioModID ModID, EModioRating Rating,
									 EModioErrorCondition ExpectedResult)
		: FModioTestLatentCommandBaseExpectedResult(AssociatedTest, ExpectedResult),
		  ModID(ModID),
          Rating(Rating)
	{}

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		Modio->SubmitModRatingAsync(ModID, Rating, FOnErrorOnlyDelegateFast::CreateSP(this, &FModioSubmitRatingAsyncCommand::Callback));
	}
	void Callback(FModioErrorCode ec)
	{
		CurrentTest->TestEqual("SubmitRating completes without errors", ec, false);
		
        Done();
	}
};

#endif