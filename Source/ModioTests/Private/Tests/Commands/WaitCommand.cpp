/* 
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "Tests/Commands/WaitCommand.h"
#include "Engine/Engine.h"

#if WITH_DEV_AUTOMATION_TESTS

bool FModioWaitCommand::ModioUpdate(float DeltaTime)
{
	if (WaitDuration > 0.0f)
	{
		if (RepeatedLogTimer <= 0.0f)
		{
			RepeatedLogTimer = 1.0f;
			AddInfo(FString::Printf(TEXT("Waiting for %f seconds"), WaitDuration));
		}
		WaitDuration -= DeltaTime;
		return false;
	}

	return true;
}

#endif