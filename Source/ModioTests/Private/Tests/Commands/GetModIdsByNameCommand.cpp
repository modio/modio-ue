/* 
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "Tests/Commands/GetModIdsByNameCommand.h"
#include "Engine/Engine.h"

#if WITH_DEV_AUTOMATION_TESTS

void FModioGetModIdsByNameCommand::Start() 
{
	if (!Result.IsValid())
	{
		AddError(TEXT("Result array is not valid"));
		Done();
	}
}

bool FModioGetModIdsByNameCommand::ModioUpdate(float DeltaTime)
{
	if (!bRequested)
	{
		return RequestModIds();
	}

	return CheckModIdsReceived();
}

bool FModioGetModIdsByNameCommand::RequestModIds()
{
	if (bRequested)
	{
		AddError(TEXT("Mod ID request was already made but Update was called again before completion"));
		return true;
	}
	UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!ModioSubsystem)
	{
		AddError(TEXT("ModioSubsystem not found"));
		return true;
	}
	for (const FString& ModName : ModNames)
	{
		FModioFilterParams FilterParams;
		FilterParams.NameContains(ModName);
		ModioSubsystem->ListAllModsAsync(
			FilterParams, FOnListAllModsDelegateFast::CreateLambda([this, WeakThis = AsWeak(),
																	ModName](FModioErrorCode ErrorCode,
																			 TOptional<FModioModInfoList> ModInfoList) {
				if (!WeakThis.IsValid())
				{
					// The command has been destroyed by the time this callback is called
					return;
				}
				if (ErrorCode)
				{
					AddError(FString::Printf(TEXT("Failed to list for mod name %s - mods: %s"), *ModName,
											 *ErrorCode.GetErrorMessage()));
					return;
				}
				if (!ModInfoList.IsSet() || ModInfoList.GetValue().GetRawList().Num() == 0)
				{
					AddError(FString::Printf(TEXT("No mods found for Mod Name %s"), *ModName));
					return;
				}
				for (const FModioModInfo& ModInfo : ModInfoList.GetValue().GetRawList())
				{
					if (ModInfo.ProfileName == ModName)
					{
						AddInfo(FString::Printf(TEXT("Found mod ID %s for mod name %s"), *ModInfo.ModId.ToString(),
												*ModName));
						Result->Add(ModInfo.ModId);
						return;
					}
				}
				AddError(FString::Printf(TEXT("Failed to find exact matching mod with name %s"), *ModName));
			}));
	}
	bRequested = true;
	return false;
}

bool FModioGetModIdsByNameCommand::CheckModIdsReceived()
{
	if (Result->Num() == ModNames.Num())
	{
		AddInfo(TEXT("Received all requested mod IDs"));
		return true;
	}

	if (RepeatedLogTimer <= 0.0f)
	{
		RepeatedLogTimer = 1.0f;
		AddInfo(FString::Printf(TEXT("Waiting for mod IDs to be received - currently received %d of %d"), Result->Num(),
								ModNames.Num()));
	}
	return false;
}

#endif