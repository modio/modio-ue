/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#pragma once
#include "ModioSubsystem.h"
#include "Tests/Commands/ModioTestCommandBase.h"

#if WITH_DEV_AUTOMATION_TESTS
class FModioEnableModManagementCommand : public FModioTestLatentCommandBase
{
FOnModManagementDelegateFast Callback;
public:
	FModioEnableModManagementCommand(FAutomationTestBase* AssociatedTest,  const FOnModManagementDelegateFast& Callback)
	:FModioTestLatentCommandBase(AssociatedTest),
	Callback(Callback){};

	using FModioTestLatentCommandBase::Update;
	
	virtual void Start() override
	{
		Modio->EnableModManagement(Callback);
		Done();
	}
	
};
#endif