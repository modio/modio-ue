/*
 *  Copyright (C) 2025-2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "ModioSubsystem.h"
#include "Types/ModioServerInitializeOptions.h"

#include "ModioMultiplayerSubsystem.generated.h"

DECLARE_DELEGATE_TwoParams(FAddClientModsDelegateFast, FModioErrorCode, TSet<FModioModID>);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FAddClientModsDelegate, FModioErrorCode, ErrorCode, TSet<FModioModID>, ModIds);

/**
 * @brief `ModioMultiplayerSubsystem` is a thin wrapper around the mod.io  Multiplayer SDK Library, wrapping all the functions available in the SDK's
 * public header `modio/detail/ModioSDKMultiplayerLibrary.h`. This subsystem also converts mod.io Multiplayer SDK types to unreal-friendly types and caches
 * some expensive operations.
 * @experimental
 */
UCLASS(MinimalAPI)
class UModioMultiplayerSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

private:

	/**
	 * @brief Internal cache of the options used to initialize this session. Specific properties
	 * are exposed as protected methods and used by friend classes for additional functionality.
	 * @experimental
	 */
	FModioServerInitializeOptions CachedServerInitializeOptions;

public:

	/** Begin USubsystem interface */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	/** End USubsystem interface */

	/**
	 * @docpublic
	 * @brief Initializes the SDK in the context of a dedicated server. Loads the state of all mods installed on the
	 * system. Loads or gets all mods indicated in the initialization param, optionally updating to the latest versions.
	 * @param ServerInitializeOptions Parameters to the function packed as a struct where all members needs to be
	 * initialized for the call to succeed
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `FilesystemError`|Couldn't create the user data or common data folders
	 * @errorcategory `ConfigurationError`|`InitializeOptions` contains an invalid value. Inspect `ec.value()` to
	 * determine what was incorrect
	 * @errorcategory `SDKAlreadyInitialized`|SDK already initialized
	 * @experimental
	 */
	MODIO_API void InitializeServerAsync(const FModioServerInitializeOptions& ServerOptions,
										 FOnErrorOnlyDelegateFast OnInitComplete);

	/**
	 * @docpublic
	 * @brief Gets and/or updates all of the indicated mods for this server, assuming you have provided a valid S2S
	 * token. Is called automatically by InitialalizeModioServerAsync() depending on UpdateModsOnInit state.
	 * @param Mods the mods to get or update
	 * @param Callback callback executed upon completion of all mods
	 * @experimental
	*/
	MODIO_API void InstallOrUpdateServerModsAsync(TArray<FModioModID> Mods, FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Adds the provided Mods to the Server's list of client mods. Intended to be called on the server.
	 * Once complete it is expected to propagate the server's list of client mods back to all clients to ensure
	 * syncronization
	 * @param ModIDs a list of mods that the joining client has
	 * @param Callback callback upon completion of adding the mods to the Server's list.
	 * @errorcategory NetworkError|Couldn't connect to the mod.io servers
	 * @experimental
	*/
	MODIO_API void RegisterClientModsWithServerAsync(TArray<FModioModID> ModIDs, FAddClientModsDelegateFast Callback);

	/**
	* @docpublic
	* @brief Clears the server's list of client mods.
	* Intended only to be called when the Server can safely flush its list of client mods, if you want to minimize
	* the number of mods you send to clients/remove mods no longer needed.
	* @experimental
	*/
	MODIO_API void ClearRegisteredClientModList();

	/**
	* @docpublic
	* @brief Returns the full list of mods that have been registered with the server via AddClientModsToServerAsync
	* @return A TSet of ModIDs representing the Mods registered with the server.
	* @experimental
	*/
	MODIO_API TSet<FModioModID> GetRegisteredClientMods();

	/**
	 * @docpublic
	 * @brief Initializes the SDK in the context of a dedicated server. Loads the state of all mods installed on the
	 * system. Loads or gets all mods indicated in the initialization param, optionally updating to the latest versions.
	 * @param ServerInitializeOptions Parameters to the function packed as a struct where all members needs to be
	 * initialized for the call to succeed
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `FilesystemError`|Couldn't create the user data or common data folders
	 * @errorcategory `ConfigurationError`|`InitializeOptions` contains an invalid value. Inspect `ec.value()` to
	 * determine what was incorrect
	 * @errorcategory `SDKAlreadyInitialized`|SDK already initialized
	 * @experimental
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "InitializeServerAsync", Category = "mod.io|Multiplayer")
	MODIO_API void K2_InitializeServerAsync(const FModioServerInitializeOptions& ServerOptions,
											FOnErrorOnlyDelegate OnInitComplete);

	/**
	 * @docpublic
	 * @brief Gets and/or updates all of the indicated mods for this server, assuming you have provided a valid S2S
	 * token. Is called automatically by InitialalizeModioServerAsync() depending on UpdateModsOnInit state.
	 * @param Mods the mods to get or update
	 * @param Callback callback executed upon completion of all mods
	 * @experimental
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "InstallOrUpdateServerModsAsync", Category = "mod.io|Multiplayer")
	MODIO_API void K2_InstallOrUpdateServerModsAsync(TArray<FModioModID> Mods, FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Adds the provided Mods to the Server's list of client mods. Intended to be called on the server.
	 * Once complete it is expected to propagate the server's list of client mods back to all clients to ensure
	 * syncronization
	 * @param ModIDs a list of mods that the joining client has
	 * @param Callback callback upon completion of adding the mods to the Server's list.
	 * @errorcategory NetworkError|Couldn't connect to the mod.io servers
	 * @experimental
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "RegisterClientModsWithServerAsync", Category = "mod.io|Multiplayer")
	MODIO_API void K2_RegisterClientModsWithServerAsync(TArray<FModioModID> ModIDs, FAddClientModsDelegate Callback);
	
	/**
	 * @docpublic
	 * @brief Clears the server's list of client mods.
	 * Intended only to be called when the Server can safely flush its list of client mods, if you want to minimize
	 * the number of mods you send to clients/remove mods no longer needed.
	 * @experimental
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ClearRegisteredClientMods", Category = "mod.io|Multiplayer")
	MODIO_API void K2_ClearRegisteredClientMods();
	
	/**
	 * @docpublic
	 * @brief Returns the full list of mods that have been registered with the server via AddClientModsToServerAsync
	 * @return A TSet of ModIDs representing the Mods registered with the server.
	 * @experimental
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetRegisteredClientMods", Category = "mod.io|Multiplayer")
	MODIO_API TSet<FModioModID> K2_GetRegisteredClientMods();
};
