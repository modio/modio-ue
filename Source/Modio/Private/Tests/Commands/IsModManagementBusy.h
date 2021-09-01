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
class FModioIsModManagementBusyCommand : public FModioTestLatentCommandBase
{
	bool Result;
public:
	FModioIsModManagementBusyCommand(FAutomationTestBase* AssociatedTest, bool DesiredResult)
		: FModioTestLatentCommandBase(AssociatedTest),
		  Result(DesiredResult){};

	using FModioTestLatentCommandBase::Update;
	virtual void Start() override
	{
		CurrentTest->TestEqual("IsModManagementBusy returns expected result", Result, Modio->IsModManagementBusy());
		Done();
	}
};
#endif