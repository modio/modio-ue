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
class FModioQueryCurrentUpdateCommand : public FModioTestLatentCommandBase
{
TOptional<FModioModProgressInfo>& Results;
public:
	
	FModioQueryCurrentUpdateCommand(FAutomationTestBase* AssociatedTest, TOptional<FModioModProgressInfo>& Results)
		: FModioTestLatentCommandBase(AssociatedTest),
		  Results(Results) {};

	using FModioTestLatentCommandBase::Update;

	virtual void Start() override
	{
		Results = Modio->QueryCurrentModUpdate();
		Done();
	}
};
#endif