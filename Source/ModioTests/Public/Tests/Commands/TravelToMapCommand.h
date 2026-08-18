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

#if WITH_DEV_AUTOMATION_TESTS

class MODIOTESTS_API FModioTravelToMapCommand : public FModioTestLatentCommandBase
{
public:
	FModioTravelToMapCommand(FAutomationTestBase* InTestInstance, const FString& InMapPath)
		: FModioTestLatentCommandBase(InTestInstance),
		  MapPath(InMapPath)
	{
		TimeoutSeconds = 5.0f;
	}

	virtual bool ModioUpdate(float DeltaTime) override;
	virtual void Start() override;

	bool CheckTravelCompletion(float DeltaTime);

	FString MapPath;
};

#endif