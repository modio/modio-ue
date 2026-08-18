/* 
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#pragma once
#include "Tests/Commands/ModioTestCommandBase.h"
#include "Types/ModioCommonTypes.h"

#if WITH_DEV_AUTOMATION_TESTS

class MODIOTESTS_API FModioSubscribeToModsCommand : public FModioTestLatentCommandBase
{
public:
	FModioSubscribeToModsCommand(FAutomationTestBase* InTestBase, TSharedPtr<TArray<FModioModID>> InModIds)
		: FModioTestLatentCommandBase(InTestBase),
		  ModIds(InModIds)
	{
		TimeoutSeconds = 60.0f;
	}

	virtual bool ModioUpdate(float DeltaTime) override;
	virtual void Start() override;
	bool RequestSubscribe();
	bool CheckSubscribeCompletion();

	TArray<FModioModID> AwaitingSubscribeModIds;
	TSharedPtr<TArray<FModioModID>> ModIds;
	bool bRequested = false;
};

#endif