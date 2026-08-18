/* 
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "Tests/Commands/UnsubscribeAllCommand.h"
#include "Engine/Engine.h"

#if WITH_DEV_AUTOMATION_TESTS

void FModioUnsubscribeAllCommand::Start()
{
	UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!ModioSubsystem)
	{
		AddError(TEXT("ModioSubsystem not found"));
		return;
	}

	AddInfo(TEXT("Requesting unsubscribe from all mods"));

	const TMap<FModioModID, FModioModCollectionEntry>& Subscriptions = ModioSubsystem->QueryUserSubscriptions();
	for (const auto& [ModID, Subscription] : Subscriptions)
	{
		if (IgnoreUnsubscribeModIds.IsValid() && IgnoreUnsubscribeModIds->Contains(ModID))
		{
			AddInfo(FString::Printf(TEXT("Ignoring unsubscribe for mod with ID %s"), *ModID.ToString()));
			continue;
		}
		AwaitingUnsubscribeModIds.Add(ModID);
	}

	const TMap<FModioModID, FModioModCollectionEntry>& Installations = ModioSubsystem->QueryUserInstallations(true);
	for (const auto& [ModID, Installation] : Installations)
	{
		if (IgnoreUnsubscribeModIds.IsValid() && IgnoreUnsubscribeModIds->Contains(ModID))
		{
			AddInfo(FString::Printf(TEXT("Ignoring unsubscribe for installed mod with ID %s"), *ModID.ToString()));
			continue;
		}
		AwaitingUnsubscribeModIds.AddUnique(ModID);
	}

	for (const auto& ModID : AwaitingUnsubscribeModIds)
	{
		AddInfo(FString::Printf(TEXT("Unsubscribing from mod with ID %s"), *ModID.ToString()));
		ModioSubsystem->UnsubscribeFromModAsync(
			ModID,
			FOnErrorOnlyDelegateFast::CreateLambda([this, WeakThis = AsWeak(), ModID](FModioErrorCode ErrorCode) {
				if (!WeakThis.IsValid())
				{
					// The command has been destroyed by the time this callback is called
					return;
				}
				if (ErrorCode)
				{
					AddError(FString::Printf(TEXT("Failed to unsubscribe from mod - error: %s"),
											 *ErrorCode.GetErrorMessage()));
				}
				else
				{
					AddInfo(FString::Printf(TEXT("Successfully unsubscribed from mod with ID %s"), *ModID.ToString()));
					AwaitingUnsubscribeModIds.Remove(ModID);
				}
			}));
	}
}

bool FModioUnsubscribeAllCommand::ModioUpdate(float DeltaTime)
{
	return CheckUnsubscribeCompletion();
}

bool FModioUnsubscribeAllCommand::CheckUnsubscribeCompletion()
{
	UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!ModioSubsystem)
	{
		AddError(TEXT("ModioSubsystem not found"));
		return true;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& Installations = ModioSubsystem->QueryUserInstallations(true);
	if (!Installations.IsEmpty())
	{
		for (const auto& [ModID, Installation] : Installations)
		{
			if (IgnoreUnsubscribeModIds.IsValid() && IgnoreUnsubscribeModIds->Contains(ModID))
			{
				continue;
			}
			if (RepeatedLogTimer <= 0.0f)
			{
				RepeatedLogTimer = 1.0f;
				AddInfo(FString::Printf(TEXT("Waiting for unsubscribe to complete - mod with ID %s is still installed"),
										*ModID.ToString()));
			}
			return false;
		}
	}

	if (AwaitingUnsubscribeModIds.IsEmpty())
	{
		AddInfo(TEXT("All mods unsubscribed successfully"));
		return true;
	}

	if (RepeatedLogTimer <= 0.0f)
	{
		RepeatedLogTimer = 1.0f;
		AddInfo(FString::Printf(TEXT("Waiting for unsubscribe to complete - currently subscribed to %d mods"),
								AwaitingUnsubscribeModIds.Num()));
	}

	return false;
}


#endif