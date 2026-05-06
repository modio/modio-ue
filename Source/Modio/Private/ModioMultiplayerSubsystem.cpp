/*
 *  Copyright (C) 2025-2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "ModioMultiplayerSubsystem.h"
#include "Async/Async.h"
#include "Engine/Engine.h"
#include "ModioSettings.h"
#include "Internal/Convert/ServerInitializeOptions.h"
#include "Libraries/ModioErrorConditionLibrary.h"

void UModioMultiplayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency(UModioSubsystem::StaticClass());
}

void UModioMultiplayerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UModioMultiplayerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
#if UE_SERVER
	return true;
#else
	return false;
#endif
}

void UModioMultiplayerSubsystem::InitializeServerAsync(const FModioServerInitializeOptions& ServerOptions,
											FOnErrorOnlyDelegateFast OnInitComplete)
{
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		ModioSubsystem->bUseBackgroundThread = ServerOptions.BaseOptions.bUseBackgroundThread;

		if (ModioSubsystem->bUseBackgroundThread && !FPlatformProcess::SupportsMultithreading())
		{
			UE_LOG(LogModio, Warning, TEXT("PlatformProcess does not support multithreading"));
			ModioSubsystem->bUseBackgroundThread = false;
		}

		if (ModioSubsystem->bUseBackgroundThread && !ModioSubsystem->BackgroundThread)
		{
			ModioSubsystem->bUseBackgroundThread = true;
			ModioSubsystem->BackgroundThread = MakeUnique<FModioBackgroundThread>(ModioSubsystem);
			ModioSubsystem->CachedInitializeOptions = ServerOptions.BaseOptions;
		}
		Modio::InitializeModioServerAsync(
			ToModio(ServerOptions),
			[WeakModioSubsystem = MakeWeakObjectPtr(ModioSubsystem), WeakThis = MakeWeakObjectPtr(this), OnInitComplete,
			 ServerOptions, this](Modio::ErrorCode ec)
				{
					if (ec)
					{
						OnInitComplete.ExecuteIfBound(FModioErrorCode(ec));
						return;
					}
					if (!WeakModioSubsystem.IsValid() || !WeakThis.IsValid())
					{
						OnInitComplete.ExecuteIfBound(FModioErrorCode(ec));
						return;
					}
					AsyncTask(ENamedThreads::GameThread, ([WeakModioSubsystem, WeakThis, OnInitComplete, ec, ServerOptions]()
						{
							if (WeakModioSubsystem.IsValid())
							{
								TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
								if (!ec && !UModioErrorConditionLibrary::ErrorCodeMatches(ec, EModioErrorCondition::SDKAlreadyInitialized))
								{
									WeakModioSubsystem->CachedInitializeOptions = ServerOptions.BaseOptions;
									WeakThis->CachedServerInitializeOptions = ServerOptions;
								}
							WeakModioSubsystem->InvalidateUserSubscriptionCache();
								OnInitComplete.ExecuteIfBound(FModioErrorCode(ec));
							}
						}));
			});
	}
	else
	{
		UE_LOG(LogModio, Error, TEXT("Could not get Modio Subsystem. Could not init server."));
	}
}

MODIO_API void UModioMultiplayerSubsystem::InstallOrUpdateServerModsAsync(TArray<FModioModID> Mods,
																		  FOnErrorOnlyDelegateFast Callback)
{
	Modio::InstallOrUpdateServerModsAsync(ToModio(Mods),
		[WeakThis = MakeWeakObjectPtr(this), Callback](Modio::ErrorCode ec) {
											  Callback.ExecuteIfBound(FModioErrorCode(ec));
		});
}

MODIO_API void UModioMultiplayerSubsystem::RegisterClientModsWithServerAsync(TArray<FModioModID> ModIDs,
																	  FAddClientModsDelegateFast Callback)
{
	Modio::RegisterClientModsWithServerAsync(
		ToModio(ModIDs), [WeakThis = MakeWeakObjectPtr(this), Callback](Modio::ErrorCode ec, std::set<Modio::ModID> ModList)
		{
			// @TODO is there an implicit way to init a TSet from a std::set? Is it quicker than this?
			TSet<FModioModID> Result;
			for (const Modio::ModID& ModId : ModList)
			{
				Result.Add(ToUnreal(ModId));
			}
		Callback.ExecuteIfBound(FModioErrorCode(ec), Result);
		});
}

MODIO_API void UModioMultiplayerSubsystem::ClearRegisteredClientModList()
{
	Modio::ClearRegisteredClientMods();
}

MODIO_API TSet<FModioModID> UModioMultiplayerSubsystem::GetRegisteredClientMods()
{
	TSet<FModioModID> Out;

	for (const Modio::ModID& Mod : Modio::GetRegisteredClientMods())
	{
		Out.Add(ToUnreal(Mod));
	}

	return Out;
}

MODIO_API void UModioMultiplayerSubsystem::K2_InitializeServerAsync(const FModioServerInitializeOptions& ServerOptions,
																	FOnErrorOnlyDelegate OnInitComplete)
{
	InitializeServerAsync(ServerOptions, FOnErrorOnlyDelegateFast::CreateLambda([OnInitComplete](FModioErrorCode ec)
		{
			OnInitComplete.ExecuteIfBound(ec);
		}));
}

MODIO_API void UModioMultiplayerSubsystem::K2_InstallOrUpdateServerModsAsync(TArray<FModioModID> Mods,
																			 FOnErrorOnlyDelegate Callback)
{
	InstallOrUpdateServerModsAsync(
		Mods, FOnErrorOnlyDelegateFast::CreateLambda([Callback](FModioErrorCode ec)
			{
				Callback.ExecuteIfBound(ec);
			}));
}

MODIO_API void UModioMultiplayerSubsystem::K2_RegisterClientModsWithServerAsync(TArray<FModioModID> ModIDs,
																				FAddClientModsDelegate Callback)
{
	RegisterClientModsWithServerAsync(
		ModIDs, FAddClientModsDelegateFast::CreateLambda([Callback](FModioErrorCode ec, TSet<FModioModID> ModIds)
			{
				Callback.ExecuteIfBound(ec, ModIds);
			}));
}

MODIO_API void UModioMultiplayerSubsystem::K2_ClearRegisteredClientMods()
{
	ClearRegisteredClientModList();
}

MODIO_API TSet<FModioModID> UModioMultiplayerSubsystem::K2_GetRegisteredClientMods()
{
	return GetRegisteredClientMods();
}
