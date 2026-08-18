/* 
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "Tests/Commands/TravelToMapCommand.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Engine/Engine.h"

bool FModioTravelToMapCommand::ModioUpdate(float DeltaTime)
{
	return CheckTravelCompletion(DeltaTime);
}

void FModioTravelToMapCommand::Start()
{
	AddInfo(FString::Printf(TEXT("Requesting travel to map %s"), *MapPath));

	if (!GWorld) 
	{
		AddError(TEXT("GWorld is null, cannot travel to map"));
		Done();
		return;
	}

	FString OpenCommand = FString::Printf(TEXT("Open %s"), *MapPath);
	GEngine->Exec(GWorld, *OpenCommand);
}

bool FModioTravelToMapCommand::CheckTravelCompletion(float DeltaTime)
{
	if (GWorld && MapPath.EndsWith(GWorld->GetMapName()))
	{
		AddInfo(FString::Printf(TEXT("Successfully traveled to map %s"), *MapPath));
		return true;
	}

	if (RepeatedLogTimer <= 0.0f)
	{
		RepeatedLogTimer = 1.0f;
		AddInfo(FString::Printf(TEXT("Waiting for travel to map %s to complete. Current Map %s. Timeout in %f"),
								*MapPath, (GWorld ? *GWorld->GetMapName() : TEXT("null")), TimeoutSeconds));
	}

	return false;
}

#endif