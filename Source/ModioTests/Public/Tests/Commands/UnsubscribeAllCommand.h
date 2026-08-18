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

class MODIOTESTS_API FModioUnsubscribeAllCommand : public FModioTestLatentCommandBase
{
public:
	FModioUnsubscribeAllCommand(FAutomationTestBase* InTestInstance,
								TSharedPtr<TArray<FModioModID>> InIgnoreUnsubscribeModIds)
		: FModioTestLatentCommandBase(InTestInstance),
		  IgnoreUnsubscribeModIds(InIgnoreUnsubscribeModIds)
	{}

	virtual bool ModioUpdate(float DeltaTime) override;
	virtual void Start() override;
	bool CheckUnsubscribeCompletion();

	TArray<FModioModID> AwaitingUnsubscribeModIds;
	TSharedPtr<TArray<FModioModID>> IgnoreUnsubscribeModIds;
};

#endif