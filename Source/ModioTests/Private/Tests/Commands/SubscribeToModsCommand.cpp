/* 
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#include "Tests/Commands/SubscribeToModsCommand.h"
#include "Engine/Engine.h"

#if WITH_DEV_AUTOMATION_TESTS

void FModioSubscribeToModsCommand::Start() 
{
	if (!ModIds.IsValid())
	{
		AddError(TEXT("Mod IDs are not valid"));
		return;
	}

	if (ModIds->IsEmpty())
	{
		AddError(TEXT("Mod IDs are empty"));
		return;
	}

	RequestSubscribe();
}

bool FModioSubscribeToModsCommand::ModioUpdate(float DeltaTime)
{
	return CheckSubscribeCompletion();
}

bool FModioSubscribeToModsCommand::RequestSubscribe()
{
	if (bRequested)
	{
		AddError(TEXT("Subscribe request was already made but Update was called again before completion"));
		return true;
	}
	UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!ModioSubsystem)
	{
		AddError(TEXT("ModioSubsystem not found"));
		return true;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& Subscriptions = ModioSubsystem->QueryUserSubscriptions();

	AwaitingSubscribeModIds = *ModIds;
	for (const FModioModID& ModId : *ModIds)
	{
		if (Subscriptions.Contains(ModId) && Subscriptions[ModId].GetModState() == EModioModState::Installed)
		{
			AddInfo(FString::Printf(TEXT("Already subscribed to mod with ID %s"), *ModId.ToString()));
			AwaitingSubscribeModIds.Remove(ModId);
			continue;
		}

		AddInfo(FString::Printf(TEXT("Subscribing to mod with ID %s"), *ModId.ToString()));
		ModioSubsystem->SubscribeToModAsync(
			ModId, true,
			FOnErrorOnlyDelegateFast::CreateLambda([this, WeakThis = AsWeak(), ModId](
													   FModioErrorCode ErrorCode) {
				if (!WeakThis.IsValid())
				{
					// The command has been destroyed by the time this callback is called
					return;
				}
				if (ErrorCode)
				{
					AddError(
						FString::Printf(TEXT("Failed to subscribe to mod - error: %s"), *ErrorCode.GetErrorMessage()));
				}
				else
				{
					AddInfo(FString::Printf(TEXT("Successfully subscribed to mod with ID %s"), *ModId.ToString()));
					AwaitingSubscribeModIds.Remove(ModId);
				}
			}));
	}
	bRequested = true;
	return false;
}

bool FModioSubscribeToModsCommand::CheckSubscribeCompletion()
{
	UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!ModioSubsystem)
	{
		AddError(TEXT("ModioSubsystem not found"));
		return true;
	}

	if (!AwaitingSubscribeModIds.IsEmpty())
	{
		if (RepeatedLogTimer <= 0.0f)
		{
			RepeatedLogTimer = 1.0f;
			AddInfo(FString::Printf(TEXT("Waiting for subscribe to complete - currently awaiting %d mods"),
									AwaitingSubscribeModIds.Num()));
		}
		return false;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& Subscriptions = ModioSubsystem->QueryUserSubscriptions();

	// Make sure all the mods we are awaiting are in the subscriptions
	for (const FModioModID& AwaitingModId : *ModIds)
	{
		if (!Subscriptions.Contains(AwaitingModId))
		{
			if (RepeatedLogTimer <= 0.0f)
			{
				RepeatedLogTimer = 1.0f;
				AddInfo(FString::Printf(TEXT("Waiting for subscribe to complete - mod with ID %s is not in subscriptions"),
										*AwaitingModId.ToString()));
			}
			return false;
		}
	}

	// Make sure all mods we are subscribed to are installed (including ones we may not have been awaiting)
	for (const auto& [ModId, Entry] : Subscriptions)
	{
		if (Entry.GetModState() != EModioModState::Installed)
		{
			if (RepeatedLogTimer <= 0.0f)
			{
				RepeatedLogTimer = 1.0f;
				const FString ModStateString = UEnum::GetValueAsString(Entry.GetModState());
				AddInfo(FString::Printf(TEXT("Waiting for subscribe to complete - mod with ID %s is in state %s"),
										*ModId.ToString(), *ModStateString));
			}
			return false;
		}
	}

	AddInfo(TEXT("All mods subscribed successfully"));
	return true;
}

#endif