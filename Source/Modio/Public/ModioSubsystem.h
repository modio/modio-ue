/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Containers/Queue.h"
#include "Misc/EngineVersionComparison.h"
#if UE_VERSION_OLDER_THAN(5, 3, 0)
	#include "Core/Public/HAL/Runnable.h"
#endif
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "ModioImageCache.h"
#include "Subsystems/EngineSubsystem.h"
#include "Templates/UniquePtr.h"
#include "Types/ModioAuthenticationParams.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioCreateModFileParams.h"
#include "Types/ModioCreateModParams.h"
#include "Types/ModioEditModParams.h"
#include "Types/ModioEntitlementConsumptionStatusList.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioFilterParams.h"
#include "Types/ModioGameInfo.h"
#include "Types/ModioGameInfoList.h"
#include "Types/ModioImageWrapper.h"
#include "Types/ModioInitializeOptions.h"
#include "Types/ModioModChangeMap.h"
#include "Types/ModioModCollectionEntry.h"
#include "Types/ModioModCreationHandle.h"
#include "Types/ModioModDependencyList.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioModInfoList.h"
#include "Types/ModioModManagementEvent.h"
#include "Types/ModioModProgressInfo.h"
#include "Types/ModioModTagOptions.h"
#include "Types/ModioRating.h"
#include "Types/ModioReportParams.h"
#include "Types/ModioStorageInfo.h"
#include "Types/ModioTerms.h"
#include "Types/ModioTransactionRecord.h"
#include "Types/ModioUser.h"
#include "Types/ModioUserList.h"
#include "Types/ModioValidationError.h"

#include "ModioSubsystem.generated.h"

// Native versions of the delegates
DECLARE_DELEGATE_OneParam(FOnErrorOnlyDelegateFast, FModioErrorCode);
DECLARE_DELEGATE_OneParam(FOnModManagementDelegateFast, FModioModManagementEvent);
DECLARE_DELEGATE_OneParam(FOnUserProfileUpdatedDelegate, TOptional<FModioUser> UserProfile);
DECLARE_DELEGATE_TwoParams(FOnListAllModsDelegateFast, FModioErrorCode, TOptional<FModioModInfoList>);
DECLARE_DELEGATE_TwoParams(FOnGetModInfoDelegateFast, FModioErrorCode, TOptional<FModioModInfo>);
DECLARE_DELEGATE_TwoParams(FOnGetGameInfoDelegateFast, FModioErrorCode, TOptional<FModioGameInfo>);
DECLARE_DELEGATE_TwoParams(FOnListUserGamesDelegateFast, FModioErrorCode, TOptional<FModioGameInfoList>);
DECLARE_DELEGATE_TwoParams(FOnGetMediaDelegateFast, FModioErrorCode, TOptional<FModioImageWrapper>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGetMediaMulticastDelegateFast, FModioErrorCode, TOptional<FModioImageWrapper>);
DECLARE_DELEGATE_TwoParams(FOnGetModTagOptionsDelegateFast, FModioErrorCode, TOptional<FModioModTagOptions>);
DECLARE_DELEGATE_TwoParams(FOnGetTermsOfUseDelegateFast, FModioErrorCode, TOptional<FModioTerms>);
DECLARE_DELEGATE_TwoParams(FOnGetModDependenciesDelegateFast, FModioErrorCode, TOptional<FModioModDependencyList>);
DECLARE_DELEGATE_TwoParams(FOnSubmitNewModDelegateFast, FModioErrorCode, TOptional<FModioModID>);
DECLARE_DELEGATE_TwoParams(FOnMuteUsersDelegateFast, FModioErrorCode, TOptional<FModioUserList>);
DECLARE_DELEGATE_TwoParams(FOnListUserCreatedModsDelegateFast, FModioErrorCode, TOptional<FModioModInfoList>);
DECLARE_DELEGATE_TwoParams(FOnPreviewExternalUpdatesDelegateFast, FModioErrorCode, TOptional<FModioModChangeMap>);
DECLARE_DELEGATE_TwoParams(FOnPurchaseModDelegateFast, FModioErrorCode, TOptional<FModioTransactionRecord>);
DECLARE_DELEGATE_TwoParams(FOnGetUserWalletBalanceDelegateFast, FModioErrorCode, TOptional<uint64>);
DECLARE_DELEGATE_OneParam(FOnFetchUserPurchasesDelegateFast, FModioErrorCode);
DECLARE_DELEGATE_TwoParams(FOnGetUserDelegationTokenDelegateFast, FModioErrorCode, FString);
DECLARE_DELEGATE_TwoParams(FOnRefreshUserEntitlementsDelegateFast, FModioErrorCode,
						   TOptional<FModioEntitlementConsumptionStatusList>);
DECLARE_DELEGATE_RetVal(EModioLanguage, FGetCurrentLanguageDelegate);

// Blueprint version of delegates

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnErrorOnlyDelegate, FModioErrorCode, ErrorCode);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnModManagementDelegate, FModioModManagementEvent, Event);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnListAllModsDelegate, FModioErrorCode, ErrorCode, FModioOptionalModInfoList,
								   Result);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetModInfoDelegate, FModioErrorCode, ErrorCode, FModioOptionalModInfo, ModInfo);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetGameInfoDelegate, FModioErrorCode, ErrorCode, FModioOptionalGameInfo,
								   GameInfo);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnListUserGamesDelegate, FModioErrorCode, ErrorCode, FModioOptionalGameInfoList,
								   GameInfoList);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetMediaDelegate, FModioErrorCode, ErrorCode, FModioOptionalImage, Path);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetModTagOptionsDelegate, FModioErrorCode, ErrorCode, FModioOptionalModTagOptions,
								   ModTagOptions);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetTermsOfUseDelegate, FModioErrorCode, ErrorCode, FModioOptionalTerms, Terms);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetModDependenciesDelegate, FModioErrorCode, ErrorCode,
								   FModioOptionalModDependencyList, Dependencies);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnSubmitNewModDelegate, FModioErrorCode, ErrorCode, FModioOptionalModID, NewModID);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnMuteUsersDelegate, FModioErrorCode, ErrorCode, FModioOptionalUserList,
								   NewUserList);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnListUserCreatedModsDelegate, FModioErrorCode, ErrorCode,
								   FModioOptionalModInfoList, Result);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPreviewExternalUpdatesDelegate, FModioErrorCode, ErrorCode,
								   FModioOptionalModChangeMap, ModioPreviewMap);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPurchaseModDelegate, FModioErrorCode, ErrorCode, FModioOptionalTransactionRecord,
								   Transaction);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetUserWalletBalanceDelegate, FModioErrorCode, ErrorCode, FModioOptionalUInt64,
								   WalletBalance);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFetchUserPurchasesDelegate, FModioErrorCode, ErrorCode);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetUserDelegationTokenDelegate, FModioErrorCode, ErrorCode, FString, Token);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnRefreshUserEntitlementsDelegate, FModioErrorCode, ErrorCode,
								   FModioOptionalEntitlementConsumptionStatusList, Entitlements);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPlatformCheckoutDelegate, bool, bSuccess, FString, Message);

class UModioSubsystem;

class FModioBackgroundThread : public FRunnable
{
public:
	FModioBackgroundThread(UModioSubsystem* ModioSubsystem);
	virtual ~FModioBackgroundThread();

	/**
	 * Safely kills the thread. This will wait for the thread to finish any pending work (in Run()) before exiting
	 */
	void KillThreadSafe();

	//~ Begin FRunnable Interface
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	//~ End FRunnable Interface

protected:
	bool bStopThread = false;

	UModioSubsystem* CurrentModioSubsystem;
	TUniquePtr<FRunnableThread> CurrentRunningThread;
};

/**
 * @brief `ModioSubsystem` is a thin wrapper around the mod.io SDK, wrapping all the functions available in the SDK's
 * public header `modio/ModioSDK.h`.  This subsystem also converts mod.io SDK types to unreal-friendly types and caches
 * some expensive operations.
 */
UCLASS(MinimalAPI)
class UModioSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

protected:
	TUniquePtr<FModioBackgroundThread> BackgroundThread;

	friend class UModioUISubsystem;
	EModioPortal GetCurrentPortal() const
	{
		return CachedInitializeOptions.PortalInUse;
	}

#if WITH_EDITOR
	/// @brief Internal method used for emitting a warning during PIE if the Plugin was initialized during that PIE
	/// session
	void CheckShutdownBeforePIEClose(UWorld*);
	bool bInitializedDuringPIE = false;
#endif

private:
	bool bUseBackgroundThread = false;

	/**
	 * @brief Internal cache of the options used to initialize this session. Specific properties
	 * are exposed as protected methods and used by friend classes for additional functionality.
	 */
	FModioInitializeOptions CachedInitializeOptions;

public:
	FOnUserProfileUpdatedDelegate OnUserProfileUpdatedDelegate;

	/** Begin USubsystem interface */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	/** End USubsystem interface */

	/// @brief Bind to a function that returns the appropriate `EModioLanguage` for the game. The Terms of Use will be
	/// displayed in this language during authentication.
	FGetCurrentLanguageDelegate GetCurrentLanguage;

	/**
	 * @docpublic
	 * @brief Returns if the background thread is being used or not
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io")
	MODIO_API bool IsUsingBackgroundThread();

	/**
	 * @docpublic
	 * @brief Kills the background thread if it is running
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io")
	MODIO_API void KillBackgroundThread();

	/**
	 * @docpublic
	 * @brief Initializes the SDK for the given user. Loads the state of all mods installed on the system as well as the
	 * set of mods the specified user has installed on this device
	 * @param InitializeOptions Parameters to the function packed as a struct where all members needs to be initialized
	 * for the call to succeed
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `FilesystemError`|Couldn't create the user data or common data folders
	 * @errorcategory `ConfigurationError`|`InitializeOptions` contains an invalid value. Inspect `ec.value()` to
	 * determine what was incorrect
	 * @errorcategory `SDKAlreadyInitialized`|SDK already initialized
	 */
	MODIO_API void InitializeAsync(const FModioInitializeOptions& InitializeOptions,
								   FOnErrorOnlyDelegateFast OnInitComplete);

	/**
	 * @docpublic
	 * @brief Sets the global logging level.
	 * @note This is a transient function, and won't be stored to disk. So at the next engine start, the log level will
	 * be restored to the one in your `ModioRuntimeSettings`
	 * @param UnrealLogLevel Determines which messages to include in the log output. Messages with a log level below the
	 * specified value will not be displayed.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io")
	MODIO_API void SetLogLevel(EModioLogLevel UnrealLogLevel);

	/**
	 * @docpublic
	 * @brief Runs any pending mod.io work on the calling thread and invokes any callbacks passed to asynchronous
	 * operations.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io")
	MODIO_API void RunPendingHandlers();

	/**
	 * @docpublic
	 * @brief Cancels any running internal operations, frees SDK resources, and invokes any pending callbacks with an
	 *OperationCanceled error category. This function will NOT block while the deinitialization occurs.
	 * @param OnShutdownComplete Callback invoked when the plugin is shut down and calling
	 *[`RunPendingHandlers`](#run-pending-handlers) is no longer required
	 * @requires initialized-sdk
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 */
	MODIO_API void ShutdownAsync(FOnErrorOnlyDelegateFast OnShutdownComplete);

	/**
	 * @docpublic
	 * @brief If the last request to the mod.io servers returned a validation failure, this function returns extended
	 * information describing the fields that failed validation.
	 * @return Collection of `ModioValidationError` objects, or empty collection if there were no validation failures
	 * @requires initialized-sdk
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io")
	MODIO_API TArray<FModioValidationError> GetLastValidationError();

	/**
	 * @docpublic
	 * @brief Sends a request to the mod.io server to add the specified mod to the user's list of subscriptions, and
	 * marks the mod for local installation by the SDK
	 * @param ModToSubscribeTo Mod ID of the mod requiring a subscription.
	 * @param IncludeDependencies Subscribe to all dependencies as well.
	 * @param OnSubscribeComplete Callback invoked when the subscription request is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod does not exist or was deleted
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	MODIO_API void SubscribeToModAsync(FModioModID ModToSubscribeTo, bool IncludeDependencies,
									   FOnErrorOnlyDelegateFast OnSubscribeComplete);

	/**
	 * @docpublic
	 * @brief Sends a request to the mod.io server to remove the specified mod from the user's list of subscriptions.
	 * If no other local users are subscribed to the specified mod, this function will also mark the mod for
	 * uninstallation by the SDK.
	 * @param ModToUnsubscribeFrom Mod ID of the mod requiring unsubscription.
	 * @param OnUnsubscribeComplete Callback invoked when the unsubscription request is complete.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod does not exist or was deleted
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	MODIO_API void UnsubscribeFromModAsync(FModioModID ModToUnsubscribeFrom,
										   FOnErrorOnlyDelegateFast OnUnsubscribeComplete);

	/**
	 * @docpublic
	 * @brief Synchronises the local list of the current user's subscribed mods with the server. Any mods that have
	 * been externally subscribed will be automatically marked for installation, and mods that have been externally
	 * removed from the user's subscriptions may be uninstalled if no other local users have a current subscription.
	 * @param OnFetchDone Callback invoked when the external state has been retrieved and merged with the local data
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 */
	MODIO_API void FetchExternalUpdatesAsync(FOnErrorOnlyDelegateFast OnFetchDone);

	/**
	 * @docpublic
	 * @brief Retrieve a list of updates between the users local mod state, and the server-side state. This allows you
	 * to identify which mods will be modified by the next call to
	 * [`FetchExternalUpdatesAsync`](#fetchexternalupdatesasync) in order to perform any content management (such as
	 * unloading files) that might be required.
	 * @param OnPreviewDone Callback invoked when the external state has been retrieved. It contains a dictionary with
	 * ModID as keys and change maps as values. Empty when there are no differences between local and the mod.io API
	 * service
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 */
	MODIO_API void PreviewExternalUpdatesAsync(FOnPreviewExternalUpdatesDelegateFast OnPreviewDone);

	/**
	 * @docpublic
	 * @brief Enables the automatic management of installed mods on the system based on the user's subscriptions.
	 * @param Callback This callback handler will be invoked with a
	 * [`ModioModManagementEvent`](#modiomodmanagementevent) for each mod operation performed by the SDK
	 * @return An error code indicating success or failure of enabling mod management.  Note that this is independent of
	 * error codes for mod management events.  Inspect the `Callback` for information on each mod management event.
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `ModManagementAlreadyEnabled`|Mod management was already enabled. The mod management callback has
	 * not been changed.
	 */
	MODIO_API FModioErrorCode EnableModManagement(FOnModManagementDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Disables automatic installation or uninstallation of mods based on the user's subscriptions. Allows
	 * currently processing installation to complete. Will cancel any pending operations when called.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Mod Management")
	MODIO_API void DisableModManagement();

	/**
	 * @docpublic
	 * @brief Checks if the automatic management process is currently installing or removing mods
	 * @return True if automatic management is currently performing an operation
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Mod Management")
	MODIO_API bool IsModManagementBusy();

	/**
	 * @docpublic
	 * @brief Cancels or suspends the current mod update, installation, or upload, and begins processing a pending
	 * operation for the specified mod ID
	 * @param ModToPrioritize The ID for the mod to prioritize
	 * @return Error code indicating the status of the prioritization request. Will be empty if the prioritization was
	 * successful or if the mod was already being processed
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid or not present in the list of pending operations
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Mod Management")
	MODIO_API FModioErrorCode PrioritizeTransferForMod(FModioModID ModToPrioritize);

	/**
	 * @docpublic
	 * @brief Provides progress information for a mod installation or update operation if one is currently in progress.
	 * @return Optional `ModioModProgressInfo` object containing information regarding the progress of the installation
	 * operation.
	 */
	MODIO_API TOptional<FModioModProgressInfo> QueryCurrentModUpdate();

	/**
	 * @docpublic
	 * @brief Fetches the local view of the user's subscribed mods, including mods that are subscribed but not
	 * yet installed
	 * @return `TMap<FModioID, FModioModCollectionEntry>` providing information about the subscribed
	 * mods
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|User")
	MODIO_API const TMap<FModioModID, FModioModCollectionEntry>& QueryUserSubscriptions();

	/**
	 * @docpublic
	 * @brief Fetches the subset of the user's subscribed mods that are installed and ready for loading
	 * @param bIncludeOutdatedMods Include subscribed mods that are installed but have an updated version on the server
	 * that has not yet been installed
	 * @return `TMap<FModioID, FModioModCollectionEntry>` providing information about the subscribed
	 * mods
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|User")
	MODIO_API TMap<FModioModID, FModioModCollectionEntry> QueryUserInstallations(bool bIncludeOutdatedMods);

	/**
	 * @docpublic
	 * @brief Fetches the currently authenticated mod.io user profile if there is one.
	 * @return Optional [`ModioUser`](#modiouser) object containing profile information
	 */
	MODIO_API TOptional<FModioUser> QueryUserProfile();

	/**
	 * @docpublic
	 * @brief Fetches all mods installed on the system, including those installed by other users.
	 * @return A `TMap<FModioModID, FModioModCollectionEntry>` of all mods installed on the system, including those
	 * installed by other users.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Mod Management")
	MODIO_API TMap<FModioModID, FModioModCollectionEntry> QuerySystemInstallations();

	/**
	 * @docpublic
	 * @brief Retrieves a snapshot of current storage related information such as space consumed by mod
	 * installations and total available space
	 * @return An `FModioStorageInfo` structure containing storage information
	 */
	MODIO_API FModioStorageInfo QueryStorageInfo();

	/*
	* @docpublic
	* @brief Get the storage quota for a given `EModioStorageLocation` if one has been set.
	* @param Location The type of storage to get the quota for.
	* @return `FModioOptionalUInt64` of the storage quota in bytes for the specified `EModioStorageLocation`. Empty if no quota is set.
	*/
	UFUNCTION(BlueprintPure, Category = "mod.io|Storage Info")
	MODIO_API FModioOptionalUInt64 GetStorageQuota(EModioStorageLocation Location);

	/**
	 * @docpublic
	 * @brief Provides a list of mods for the current game that match the parameters specified in the filter
	 * @param Filter [`ModioFilterParams`](#modiofilterparams) object containing any filters that should be applied to
	 * the query
	 * @param Callback Callback invoked with a status code and an optional `ModioModInfoList` providing mod profiles
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 */
	MODIO_API void ListAllModsAsync(const FModioFilterParams& Filter, FOnListAllModsDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Provides a list of games for the current user that match the parameters specified in the filter
	 * @param Filter [`ModioFilterParams`](#modiofilterparams) object containing any filters that should be applied to
	 * the query
	 * @param Callback Callback invoked with a status code and an optional `ModioModInfoList` providing mod profiles
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 */
	MODIO_API void ListUserGamesAsync(const FModioFilterParams& Filter, FOnListUserGamesDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Fetches detailed information about the specified game
	 * @param GameID Game ID of the game data to fetch
	 * @param Callback Callback providing a status code and an optional `ModioGameInfo` object with the game's extended
	 * information
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `EntityNotFoundError`|Specified game does not exist or was deleted
	 * @errorcategory `InvalidArgsError`|The supplied game ID is invalid
	 */
	MODIO_API void GetGameInfoAsync(FModioGameID GameID, FOnGetGameInfoDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Fetches detailed information about the specified mod, including description and file metadata for the
	 * most recent release
	 * @param ModId Mod ID of the mod to fetch data
	 * @param Callback Callback providing a status code and an optional `ModioModInfo` object with the mod's extended
	 * information
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod does not exist or was deleted
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	MODIO_API void GetModInfoAsync(FModioModID ModId, FOnGetModInfoDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Downloads the logo for the specified mod. Will use existing file if it is already present on disk
	 * @param ModId Mod ID for use in logo retrieval
	 * @param LogoSize Parameter indicating the size of logo that's required
	 * @param Callback Callback providing a status code and an optional path object pointing to the location of the
	 * downloaded image
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod media does not exist or was deleted
	 * @errorcategory `InsufficientSpace`|Not enough space for the file
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	MODIO_API void GetModMediaAsync(FModioModID ModId, EModioLogoSize LogoSize, FOnGetMediaDelegateFast Callback);

private:
	TMap<TTuple<FModioModID, EModioLogoSize>, FOnGetMediaMulticastDelegateFast> PendingModMediaLogoRequests;

public:
	/**
	 * @docpublic
	 * @brief Get a gallery image for the specified mod ID. If it already exists on disk the file will be reused unless
	 * it is outdated
	 * @param ModId The mod you want to retrieve an image for
	 * @param GallerySize Size of the image you want to retrieve
	 * @param Index The zero-based index of the image you want to retrieve
	 * @param Callback Callback containing a status code and an Optional containing a path to the image file on disk
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod media does not exist or was deleted
	 * @errorcategory `InsufficientSpace`|Not enough space for the file
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	MODIO_API void GetModMediaAsync(FModioModID ModId, EModioGallerySize GallerySize, int32 Index,
									FOnGetMediaDelegateFast Callback);

private:
	TMap<TTuple<FModioModID, EModioGallerySize, int32>, FOnGetMediaMulticastDelegateFast>
		PendingModMediaGalleryRequests;

public:
	/**
	 * @docpublic
	 * @brief Downloads the creator avatar for a specified mod. Will use existing file if it is already present on disk
	 * and not outdated
	 * @param ModId ID of the mod the creator avatar will be retrieved for
	 * @param AvatarSize Parameter indicating the size of avatar image that's required
	 * @param Callback Callback providing a status code and an optional path object pointing to the location of the
	 * downloaded image
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod media does not exist or was deleted
	 * @errorcategory `InsufficientSpace`|Not enough space for the file
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	MODIO_API void GetModMediaAsync(FModioModID ModId, EModioAvatarSize AvatarSize, FOnGetMediaDelegateFast Callback);

private:
	TMap<TTuple<FModioModID, EModioAvatarSize>, FOnGetMediaMulticastDelegateFast> PendingModMediaAvatarRequests;

public:
	/**
	 * @docpublic
	 * @brief Fetches the available tags used on mods for the current game. These tags can them be used in conjunction
	 * with the [`ModioFilterParams`](#modiofilterparams) passed to [`ListAllModsAsync`](#listallmodsasync)
	 * Will be cached when first received
	 * @param Callback Callback providing a status code and an optional `ModioModTagOptions` object containing the
	 *available tags
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 */
	MODIO_API void GetModTagOptionsAsync(FOnGetModTagOptionsDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief For a given Mod ID, fetches a list of any mods that the creator has marked as dependencies
	 * @param ModID The mod to retrieve dependencies for
	 * @param Recursive Include child dependencies in a recursive manner to a maximum depth of 5.
	 * @param Callback Callback providing a status code and an optional `ModioModTagOptions` object containing the
	 *available tags
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 * @experimental
	 */
	MODIO_API void GetModDependenciesAsync(FModioModID ModID, bool Recursive,
										   FOnGetModDependenciesDelegateFast Callback);

	/**
	 * @brief Adds dependencies to a specified mod, linking it with other mods that are required for it to function
	 * @param ModID The mod to add dependencies to
	 * @param Dependencies List of mod dependencies to add
	 * @param Callback Callback providing a status code to indicate if the dependencies were added successfully
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod could not be found
	 * @error UserDataError::InvalidUser|No authenticated user
	 * @error HttpError::RateLimited|Too many frequent calls to the API. Wait some time and try again.
	 * @error GenericError::BadParameter|The supplied mod ID is invalid
	 */
	MODIO_API void AddModDependenciesAsync(FModioModID ModID, const TArray<FModioModID>& Dependencies,
										   FOnErrorOnlyDelegateFast Callback);

	/**
	 * @brief Deletes dependencies from a specified mod, unlinking it from other mods that are no longer required.
	 * @param ModID The mod to delete dependencies from
	 * @param Dependencies List of mod IDs to delete as dependencies
	 * @param Callback Callback providing a status code to indicate if the dependencies were deleted successfully
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod could not be found
	 * @error UserDataError::InvalidUser|No authenticated user
	 * @error HttpError::RateLimited|Too many frequent calls to the API. Wait some time and try again.
	 * @error GenericError::BadParameter|The supplied mod ID is invalid
	 */
	MODIO_API void DeleteModDependenciesAsync(FModioModID ModID, const TArray<FModioModID>& Dependencies,
											  FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Gets a new mod handle for use with [`SubmitNewModAsync`](#submitnewmodasync).
	 * @returns New handle
	 * @experimental
	 */
	MODIO_API FModioModCreationHandle GetModCreationHandle();

	/**
	 * @docpublic
	 * @brief Requests the creation of a new mod on the server with the specified parameters
	 * @param Handle The `ModioModCreationHandle` for this submission. Once this method invokes your callback indicating
	 * success, the `ModioModCreationHandle` is invalid for the rest of the session and you should request a new one for
	 * the next submission attempt.
	 * @param Params Information about the new mod to create
	 * @param Callback Callback providing a status code and an optional `ModioModID` for the newly created mod
	 * @experimental
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `InvalidArgsError`|Some fields in `Params` did not pass validation
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 */
	MODIO_API void SubmitNewModAsync(FModioModCreationHandle Handle, FModioCreateModParams Params,
									 FOnSubmitNewModDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Queues the upload of a new modfile release for the specified mod using the submitted parameters. This
	 * function takes an `ModioCreateModFileParams` object to specify the path to the root folder of the new modfile.
	 * The plugin will compress the folder's contents into a .zip archive and queue the result for upload. When the
	 * upload completes, a mod management event will be triggered. Note the plugin is also responsible for decompressing
	 * the archive upon its installation at a later point in time.
	 * @param Mod The ID of the mod you are submitting a file for
	 * @param Params Information about the mod file being created, including the root path of the directory that will
	 * be archived
	 * @experimental
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	MODIO_API void SubmitNewModFileForMod(FModioModID Mod, FModioCreateModFileParams Params);

	/**
	 * @docpublic
	 * @brief Edits the parameters of a mod by updating any fields set in the `Params` object to match the passed-in
	 * values. Fields left empty on the `Params` object will not be updated.
	 * @param Mod The ID of the mod you wish to edit
	 * @param Params Descriptor containing the fields that should be altered.
	 * @param Callback The callback invoked when the changes have been submitted containing an optional updated
	 * `ModioModInfo` object if the edits were performed successfully
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `InvalidArgsError`|Some fields in `Params` did not pass validation
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	MODIO_API void SubmitModChangesAsync(FModioModID Mod, FModioEditModParams Params,
										 FOnGetModInfoDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Begins email authentication for the current session by requesting a one-time code be sent to the
	 * specified email address.
	 * @param EmailAddress The email address to send the code to
	 * @param Callback Callback providing a status code indicating the outcome of the request
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 */
	MODIO_API void RequestEmailAuthCodeAsync(const FModioEmailAddress& EmailAddress, FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Completes email authentication for the current session by submitting the one-time code sent to the user's
	 * email address
	 * @param AuthenticationCode User's authentication code
	 * @param Callback Callback providing a status code indicating if authentication was successful or not
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserAlreadyAuthenticatedError`|Current user is already authenticated. De-authenticate the current
	 * user with [`ClearUserDataAsync`](#clearuserdataasync), and re-initialize the SDK by calling
	 * [`ShutdownAsync`](#shutdownasync) followed by [`InitializeAsync`](#initializeasync).
	 */
	MODIO_API void AuthenticateUserEmailAsync(const FModioEmailAuthCode& AuthenticationCode,
											  FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Uses platform-specific authentication to associate a mod.io user account with the current platform user
	 * @param User Authentication payload data to submit to the provider.
	 * @param Provider The provider to use to perform the authentication
	 * @param Callback Callback invoked once the authentication request has been made
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `ConfigurationError`|The SDK's configuration is not valid
	 * @errorcategory `InvalidArgsError`|The arguments passed to the function have failed validation
	 * @errorcategory `UserTermsOfUseError`|The user has not yet accepted the mod.io Terms of Use
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserAlreadyAuthenticatedError`|Current user is already authenticated. De-authenticate the current
	 * user with [`ClearUserDataAsync`](#clearuserdataasync), and re-initialize the SDK by calling
	 * [`ShutdownAsync`](#shutdownasync) followed by [`InitializeAsync`](#initializeasync).
	 */
	MODIO_API void AuthenticateUserExternalAsync(const FModioAuthenticationParams& User,
												 EModioAuthenticationProvider Provider,
												 FOnErrorOnlyDelegateFast Callback);
	/**
	 * @docpublic
	 * @brief Queries the server to verify the state of the currently authenticated user if there is one present.
	 * @param Callback Callback invoked with the results of the verification process. An empty `ModioErrorCode`
	 * indicates successful verification i.e. the mod.io server was contactable and the user's authentication remains
	 * valid.
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 */
	MODIO_API void VerifyUserAuthenticationAsync(FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief This function retrieves the information required for a game to display the mod.io terms of use to a
	 * player who wishes to create a mod.io account
	 * @param Callback Callback invoked with the terms of use data once retrieved from the server
	 * @requires initialized-sdk
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 */
	MODIO_API void GetTermsOfUseAsync(FOnGetTermsOfUseDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Set language to get corresponding data from the server
	 * @param Locale Language to set
	 */
	MODIO_API void SetLanguage(EModioLanguage Locale);

	/**
	 * @docpublic
	 * @brief Get the currently applied language
	 * @return Current language
	 */
	MODIO_API EModioLanguage GetLanguage();

	/**
	 * @docpublic
	 * @brief De-authenticates the current mod.io user for the current session, and clears all user-specific data
	 * stored on the current device. Any mods that do not have other local users subscribed will be uninstalled
	 * @param Callback Callback providing a status code indicating the outcome of clearing the user data. Error codes
	 * returned by this function are informative only - it will always succeed.
	 */
	MODIO_API void ClearUserDataAsync(FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Downloads the avatar of the currently authenticated user. Will only perform a download if there is no
	 * local cache of the avatar or if that cached copy is out-of-date.
	 * @param AvatarSize Parameter specifying the size of avatar image to download
	 * @param Callback Callback providing a status code for the download and an optional path to the downloaded image
	 */
	MODIO_API void GetUserMediaAsync(EModioAvatarSize AvatarSize, FOnGetMediaDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Purchases a mod for the current player
	 * @param ModID ID of the mod to purchase
	 * @param ExpectedPrice The price the user is expected to pay for the mod, generally
	 * [`ModioModInfo.Price`](#modiomodinfo) . This ensures that there is consistency between the displayed price and
	 *the price in the backend. If there is a mismatch, the purchase will fail.
	 * @param Callback Callback invoked with purchase information once the purchase is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 */
	MODIO_API void PurchaseModAsync(FModioModID ModID, uint64 ExpectedPrice, FOnPurchaseModDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Gets the users current wallet balance. This will also create a wallet for a user if one does not exist.
	 * You should ensure this is called prior to calling [`PurchaseModAsync`](#purchasemodasync)
	 * purchase will fail.
	 * @param Callback Callback invoked with the users wallet balance
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 */
	MODIO_API void GetUserWalletBalanceAsync(FOnGetUserWalletBalanceDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Fetches the user's purchases. This populates a runtime cache of purchase information
	 * that can be accessed using [`QueryUserPurchasedMods`](#query-user-purchased-mods).
	 * @param Callback Callback invoked once the call has been completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 */
	MODIO_API void FetchUserPurchasesAsync(FOnFetchUserPurchasesDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Get a user delegation token that can be used for S2S service calls.
	 * @param Callback Callback invoked once the call has been completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 */
	MODIO_API void GetUserDelegationTokenAsync(FOnGetUserDelegationTokenDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Returns the user's purchased mods. [`FetchUserPurchasesAsync`](#fetchuserpurchasesasync) must be called
	 *first to populate the cache.
	 * @return A `TMap<FModioModID, FModioModInfo>` of all purchases a user has made.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Monetization")
	MODIO_API TMap<FModioModID, FModioModInfo> QueryUserPurchasedMods();

private:
	TMap<EModioAvatarSize, FOnGetMediaMulticastDelegateFast> PendingUserMediaRequests;

public:
	/**
	 * @docpublic
	 * @brief Submits a rating for a mod on behalf of the current user. Submit a neutral rating to effectively clear a
	 * rating already submitted by a user. Submitting other values will overwrite any existing rating submitted by this
	 * user.
	 * @param Mod The mod the user is rating
	 * @param Rating The rating the user wishes to submit
	 * @param Callback Callback providing a status code indicating success or failure of the rating submission
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod does not exist or was deleted
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	MODIO_API void SubmitModRatingAsync(FModioModID Mod, EModioRating Rating, FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Sends a content report to mod.io. When using this function, please inform your users that if they provide
	 * their contact name or details in the `Report` parameter, this data may be shared with the person responsible for
	 * the content being reported. For more information on what data in a report will be shared with whom, please see
	 * [our website's report form](https://mod.io/report).
	 * @param Report Information about the content being reported and a description of the report.
	 * @param Callback Callback providing a status code to indicate successful submission of the report.
	 * @requires initialized-sdk
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `InvalidArgsError`|Required information in the report did not pass validation
	 * @errorcategory `InvalidArgsError`|The mod ID, game ID, or user ID supplied to `Report` is invalid
	 */
	MODIO_API void ReportContentAsync(FModioReportParams Report, FOnErrorOnlyDelegateFast Callback);

	/** Get our image cache */
	struct FModioImageCache& GetImageCache() const;

	/**
	 * @docpublic
	 * @brief Archives a mod. This mod will no longer be able to be viewed or retrieved via the SDK, but it will still
	 * exist should you choose to restore it at a later date. Archiving is restricted to team managers and
	 * administrators only. Note that restoration and permanent deletion of a mod is possible only via web interface.
	 * @param Mod The mod to be archived.
	 * @param Callback Callback providing a status code indicating success or failure of archiving the mod.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `InsufficientPermissions`|The authenticated user does not have permission to archive this mod.
	 * This action is restricted to team managers and administrators only.
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod does not exist or was deleted
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	MODIO_API void ArchiveModAsync(FModioModID Mod, FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Forcibly uninstalls a mod from the system. This can be used when the host application requires additional
	 * space for other mods. The current user must not be subscribed to the mod to force uninstall. To remove a mod the
	 * current user is subscribed to, first use [`UnsubscribeFromModAsync`](#unsubscribefrommodasync). If the mod does
	 * not uninstall (due to a different user on the same system remaining subscribed), `ForceUninstallModAsync` can be
	 * called next.
	 * @param ModToRemove The mod ID to force uninstall.
	 * @param Callback Callback indicating success or failure of the uninstallation.
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `ApiErrorRefSuccess`|User is still subscribed to the specified mod
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	MODIO_API void ForceUninstallModAsync(FModioModID ModToRemove, FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Mute a user. This will prevent mod.io servers from returning mods authored by the muted user.
	 * when performing searches.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @param UserID ID of the User to mute
	 * @param Callback Callback providing a status code indicating success or failure of muting the user.
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied user ID is invalid
	 */
	MODIO_API void MuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Unmute a user. This allows mod.io to display mods authored by the now unmuted user when performing
	 * searches.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @param UserID ID of the user to unmute
	 * @param Callback Callback providing a status code indicating success or failure of unmuting the user.
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied user ID is invalid
	 */
	MODIO_API void UnmuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief List all the users that have been muted by the current user.
	 * @param Callback Callback providing a status code indicating success or failure of the operation, and an
	 * `Optional` containing a list of muted users if successful.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 */
	MODIO_API void GetMutedUsersAsync(FOnMuteUsersDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Provides a list of mods that the user has submitted or is a team member of for the current game,
	 * applying the parameters specified in the filter.
	 * @param Filter Filter to apply to listing the user's created mods.
	 * @param Callback Callback invoked when the call succeeds, or when an error occurs.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `RateLimited`|Too many frequent calls to the API. Wait some time and try again.
	 */
	MODIO_API void ListUserCreatedModsAsync(FModioFilterParams Filter, FOnListUserCreatedModsDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Returns the default mod installation directory for this game and platform, ignoring overrides and without
	 * requiring the SDK to be initialized.
	 * @param GameID The `ModioGameID` of the game that we're fetching the default mod installation directory for.
	 * @return The default mod installation directory for the specified game on the current platform
	 */
	static MODIO_API FString GetDefaultModInstallationDirectory(FModioGameID GameID);

	/**
	 * @docpublic
	 * @brief Returns the default mod installation directory for this game and platform, ignoring overrides and without
	 * requiring the SDK to be initialized. This overload takes an `int64` for the `GameID` param, allowing other
	 * modules to execute this function via a delegate without depending on this module and the `ModioGameID` type.
	 * @param GameID An `int64` representing the `GameID` of the game that we're fetching the default mod installation
	 * directory for.
	 * @return The default mod installation directory for the specified game on the current platform
	 */
	static MODIO_API FString GetDefaultModInstallationDirectory(int64 GameID);

	/**
	 * @docpublic
	 * @brief Install every temp mod specified by `ModIds` if not already installed.
	 * @param ModIds TArray of `ModioModID`s to install as temp mods
	 * @return Error code indicating the status of the `TempModSet`. Will be empty if it was successful
	 * @requires initialized-sdk
	 * @requires mod-management-enabled
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `ModManagementDisabled`|Mod management not enabled
	 */
	MODIO_API FModioErrorCode InitTempModSet(TArray<FModioModID> ModIds);

	/**
	 * @docpublic
	 * @brief  Add mods to a temp mod set.  Every temp mod specified by `ModIds` will be installed if not already
	 * installed.
	 * @param ModIds TArray of `ModioModID`s to install as temp mods
	 * @return Error code indicating the status of the `TempModSet`. Will be empty if it was successful
	 * @requires initialized-sdk
	 * @requires mod-management-enabled
	 * @requires temp-mod-set-initialized
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `ModManagementDisabled`|Mod management not enabled
	 * @errorcategory `TempModSetNotInitialized`|`TempModSet` not initialized
	 */
	MODIO_API FModioErrorCode AddToTempModSet(TArray<FModioModID> ModIds);

	/**
	 * @docpublic
	 * @brief  Remove mods from a temp mod set.  Every temp mod specified by `ModIds` will be uninstalled unless the
	 * user is already subscribed.
	 * @param ModIds TArray of `ModioModID`s to remove as temp mods
	 * @return Error code indicating the status of the `TempModSet`. Will be empty if it was successful
	 * @requires initialized-sdk
	 * @requires mod-management-enabled
	 * @requires temp-mod-set-initialized
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `ModManagementDisabled`|Mod management not enabled
	 * @errorcategory `TempModSetNotInitialized`|`TempModSet` not initialized
	 */
	MODIO_API FModioErrorCode RemoveFromTempModSet(TArray<FModioModID> ModIds);

	/**
	 * @docpublic
	 * @brief  Uninstall every temp mod unless the user is subscribed.
	 * @return Error code indicating the status of the `TempModSet`. Will be empty if it was successful
	 * @requires initialized-sdk
	 * @requires mod-management-enabled
	 * @requires temp-mod-set-initialized
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `ModManagementDisabled`|Mod management not enabled
	 * @errorcategory `TempModSetNotInitialized`|`TempModSet` not initialized
	 */
	MODIO_API FModioErrorCode CloseTempModSet();

	/**
	 * @docpublic
	 * @brief	Query every system and temp mod in `TempModSet`
	 * @return `TMap<FModioModID, FModioModCollectionEntry>` providing information about mods in `TempModSet`
	 */
	MODIO_API TMap<FModioModID, FModioModCollectionEntry> QueryTempModSet();

	/**
	 * @docpublic
	 * @brief Requests mod.io refresh the available entitlements for the current user purchased through the portal and
	 * currently authenticated.
	 * @param Params Additional parameters.
	 * @param Callback Callback providing an error code indicating success or failure of the refresh operation.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 */
	MODIO_API void RefreshUserEntitlementsAsync(const FModioEntitlementParams& Params,
												FOnRefreshUserEntitlementsDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Start a metrics play session
	 * @param Params `ModioMetricsSessionParams` struct containing information of what and how to start a metrics
	 * session
	 * @param Callback Callback providing an error code indicating success or failure of the session start operation
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `RateLimited`|Too many frequent calls to the API. Wait some time and try again.
	 * @errorcategory `InvalidUser`|No authenticated user
	 * @errorcategory `SessionNotInitialized`|Metrics session has not yet been initialized
	 * @errorcategory `SessionIsActive`|Metrics session is currently active and running
	 * @errorcategory `BadParameter`|One or more values in the `ModioMetricsSessionParams` are invalid
	 * @premiumfeature Metrics
	 */
	MODIO_API void MetricsSessionStartAsync(const FModioMetricsSessionParams& Params,
											FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Sends a single heartbeat to the mod.io server to indicate a session is still active
	 * @param Callback Callback providing an error code indicating success or failure of the session heartbeat
	 * operation
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `InvalidUser`|No authenticated user
	 * @errorcategory `SessionNotInitialized`|Metrics session has not yet been initialized
	 * @errorcategory `SessionIsNotActive`|Metrics session is not currently running. Call
	 * [`MetricsSessionStartAsync`](#metricssessionstartasync) before attempting to sending a heartbeat.
	 * @premiumfeature Metrics
	 */
	MODIO_API void MetricsSessionSendHeartbeatOnceAsync(FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Sends a constant heartbeat at a given interval to the mod.io server to indicate a session is still active
	 * @param IntervalSeconds The frequency in seconds to send a heartbeat to the mod.io server
	 * @param Callback Callback providing an error code indicating success or failure of the session heartbeat
	 * operation
	 *
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `InvalidUser`|No authenticated user
	 * @errorcategory `SessionNotInitialized`|Metrics session has not yet been initialized
	 * @errorcategory `SessionIsNotActive`|Metrics session is not currently running.
	 * Call [`MetricsSessionStartAsync`](#metricssessionstartasync) before attempting to sending a heartbeat.
	 * @premiumfeature Metrics
	 */
	MODIO_API void MetricsSessionSendHeartbeatAtIntervalAsync(FModioUnsigned64 IntervalSeconds,
															  FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Ends a metrics play session
	 * @param Callback Callback providing an error code indicating success or failure of the session end operation
	 *
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `RateLimited`|Too many frequent calls to the API. Wait some time and try again.
	 * @errorcategory `InvalidUser`|No authenticated user
	 * @errorcategory `SessionNotInitialized`|Metrics session has not yet been initialized
	 * @errorcategory `SessionIsNotActive`|Metrics session is not currently running.
	 * Call [`MetricsSessionStartAsync`](#metricssessionstartasync) before attempting to end a session.
	 * @premiumfeature Metrics
	 */
	MODIO_API void MetricsSessionEndAsync(FOnErrorOnlyDelegateFast Callback);

private:
	TUniquePtr<struct FModioImageCache> ImageCache;

	void InvalidateUserSubscriptionCache();

private:
	/** Maps two letter ISO 639-1 language codes to EModioLanguage values */
	TMap<FString, EModioLanguage> LanguageMap;

	/** Cached mod tags, they change so infrequently that we cache them until shutdown */
	TOptional<FModioModTagOptions> CachedModTags;

	/** Cached list of user subscriptions */
	TOptional<TMap<FModioModID, FModioModCollectionEntry>> CachedUserSubscriptions;

	UPROPERTY()
	TMap<FModioModID, FModioModCollectionEntry> UserSubscriptionsDefaultValue;

	UPROPERTY()
	TMap<FModioModID, FModioModCollectionEntry> QueryUserInstallationsDefaultValue;

	/** Cached list of mod update */
	TOptional<FModioModProgressInfo> CachedCurrentModUpdate;

	/** Cached user profile */
	TOptional<FModioUser> CachedUserProfile;

public:
#pragma region Blueprint methods

	/**
	 * @docpublic
	 * @brief Initializes the SDK for the given user. Loads the state of mods installed on the system as well as the
	 * set of mods the specified user has installed on this device
	 * @param InitializeOptions Parameters to the function packed as a struct where all members needs to be initialized
	 * for the call to succeed
	 * @param OnInitComplete Callback which will be invoked with the result of initialization
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `FilesystemError`|Couldn't create the user data or common data folders
	 * @errorcategory `ConfigurationError`|InitializeOptions contains an invalid value - inspect `ec.value()` to
	 * determine what was incorrect
	 * @errorcategory `SDKAlreadyInitialized`|SDK already initialized
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "InitializeAsync", Category = "mod.io")
	MODIO_API void K2_InitializeAsync(const FModioInitializeOptions& InitializeOptions,
									  FOnErrorOnlyDelegate OnInitComplete);

	/**
	 * @docpublic
	 * @brief Sends a request to the mod.io server to add the specified mod to the user's list of subscriptions, and
	 * marks the mod for local installation by the SDK
	 * @param ModToSubscribeTo Mod ID of the mod requiring a subscription.
	 * @param IncludeDependencies Subscribe to all dependencies as well.
	 * @param OnSubscribeComplete Callback invoked when the subscription request is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod does not exist or was deleted
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubscribeToModAsync", Category = "mod.io|Mods")
	MODIO_API void K2_SubscribeToModAsync(FModioModID ModToSubscribeTo, bool IncludeDependencies,
										  FOnErrorOnlyDelegate OnSubscribeComplete);

	/**
	 * @docpublic
	 * @brief Cancels any running internal operations, frees SDK resources, and invokes any pending callbacks with
	 * an `OperationCanceled` error category. This function will NOT block while the deinitialization occurs.
	 * @param OnShutdownComplete Callback invoked when the plugin is shut down and calling
	 *[`RunPendingHandlers`](#run-pending-handlers) is no longer required
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ShutdownAsync", Category = "mod.io")
	MODIO_API void K2_ShutdownAsync(FOnErrorOnlyDelegate OnShutdownComplete);

	/**
	 * @docpublic
	 * @brief Sends a request to the mod.io server to remove the specified mod from the user's list of subscriptions.
	 * If no other local users are subscribed to the specified mod this function will also mark the mod for
	 * uninstallation by the SDK.
	 * @param ModToUnsubscribeFrom Mod ID of the mod requiring unsubscription.
	 * @param OnUnsubscribeComplete Callback invoked when the unsubscription request is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod does not exist or was deleted
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "UnsubscribeFromModAsync", Category = "mod.io|Mods")
	MODIO_API void K2_UnsubscribeFromModAsync(FModioModID ModToUnsubscribeFrom,
											  FOnErrorOnlyDelegate OnUnsubscribeComplete);

	/**
	 * @docpublic
	 * @brief Synchronises the local list of the current user's subscribed mods with the server. Any mods that have
	 * been externally subscribed will be automatically marked for installation, and mods that have been externally
	 * removed from the user's subscriptions may be uninstalled if no other local users have a current subscription.
	 * @param OnFetchDone Callback invoked when the external state has been retrieved and merged with the local data
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "FetchExternalUpdatesAsync", Category = "mod.io|Mod Management")
	MODIO_API void K2_FetchExternalUpdatesAsync(FOnErrorOnlyDelegate OnFetchDone);

	/**
	 * @docpublic
	 * @brief Retrieve a list of updates between the users local mod state, and the server-side state. This allows
	 * you to identify which mods will be modified by the next call to
	 *[`FetchExternalUpdatesAsync`](#fetchexternalupdatesasync) in order to perform any content management (such as
	 *unloading files) that might be required.
	 * @param OnPreviewDone Callback invoked when the external state has been retrieved. It contains a dictionary with
	 * ModID as keys and change maps as values. Empty when there are no differences between local and the mod.io API
	 *service
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "PreviewExternalUpdatesAsync", Category = "mod.io|Mod Management")
	MODIO_API void K2_PreviewExternalUpdatesAsync(FOnPreviewExternalUpdatesDelegate OnPreviewDone);

	/**
	 * @docpublic
	 * @brief Enables the automatic management of installed mods on the system based on the user's subscriptions.
	 * @param Callback This callback handler will be invoked with a
	 * [`ModioModManagementEvent`](#modiomodmanagementevent) for each mod operation performed by the SDK
	 * @return An error code indicating success or failure of enabling mod management.  Note that this is independent of
	 * error codes for mod management events.  Inspect the `Callback` for information on each mod management event.
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `ModManagementAlreadyEnabled`|Mod management was already enabled. The mod management callback has
	 * not been changed.
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "EnableModManagement", Category = "mod.io|Mod Management")
	MODIO_API FModioErrorCode K2_EnableModManagement(FOnModManagementDelegate Callback);

	/**
	 * @docpublic
	 * @brief Provides progress information for a mod installation or update operation if one is currently in progress.
	 * @return `ModioOptionalModProgressInfo` object containing information regarding the progress of the installation
	 * operation.
	 */
	UFUNCTION(BlueprintPure, DisplayName = "QueryCurrentModUpdate", Category = "mod.io|Mod Management")
	MODIO_API FModioOptionalModProgressInfo K2_QueryCurrentModUpdate();

	/**
	 * @docpublic
	 * @brief Forcibly uninstalls a mod from the system. This can be used when the host application requires additional
	 * space for other mods. The current user must not be subscribed to the mod to force uninstall. To remove a mod the
	 * current user is subscribed to, first use [`UnsubscribeFromModAsync`](#unsubscribefrommodasync). If the mod does
	 * not uninstall (due to a different user on the same system remaining subscribed), `ForceUninstallModAsync` can be
	 * called next.
	 * @param ModToRemove The mod ID to force uninstall.
	 * @param Callback Callback invoked indicating success or failure of the uninstallation.
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory ApiErrorRefSuccess|User is still subscribed to the specified mod
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ForceUninstallModAsync", Category = "mod.io|Mod Management")
	MODIO_API void K2_ForceUninstallModAsync(FModioModID ModToRemove, FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Fetches the currently authenticated mod.io user profile if there is one
	 * @return `ModioOptionalUser` object containing profile information
	 */
	UFUNCTION(BlueprintPure, DisplayName = "QueryUserProfile", Category = "mod.io|User")
	MODIO_API FModioOptionalUser K2_QueryUserProfile();

	/**
	 * @docpublic
	 * @brief Provides a list of mods for the current game that match the parameters specified in the filter
	 * @param Filter [`ModioFilterParams`](#modiofilterparams) object containing any filters that should be applied to
	 * the query
	 * @param Callback Callback invoked with a status code and an optional `ModioModInfoList` providing mod profiles
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ListAllModsAsync", Category = "mod.io|Mods")
	MODIO_API void K2_ListAllModsAsync(const FModioFilterParams& Filter, FOnListAllModsDelegate Callback);

	/**
	 * @docpublic
	 * @brief Provides a list of games for the current user that match the parameters specified in the filter
	 * @param Filter [`ModioFilterParams`](#modiofilterparams) object containing any filters that should be applied to
	 * the query
	 * @param Callback Callback invoked with a status code and an optional `ModioGameInfoList` providing game profiles
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ListUserGamesAsync", Category = "mod.io|Mods")
	MODIO_API void K2_ListUserGamesAsync(const FModioFilterParams& Filter, FOnListUserGamesDelegate Callback);

	/**
	 * @docpublic
	 * @brief Fetches detailed information about the specified game
	 * @param GameID Game ID of the game data to fetch
	 * @param Callback Callback providing a status code and an optional `ModioGameInfo` object with the game's extended
	 * information
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `EntityNotFoundError`|Specified game does not exist
	 * @errorcategory `InvalidArgsError`|The supplied game ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetGameInfoAsync", Category = "mod.io|Game")
	MODIO_API void K2_GetGameInfoAsync(FModioGameID GameID, FOnGetGameInfoDelegate Callback);

	/**
	 * @docpublic
	 * @brief Fetches detailed information about the specified mod, including description and file metadata for the
	 * most recent release
	 * @param ModId Mod ID of the mod to fetch data
	 * @param Callback Callback providing a status code and an optional `ModioModInfo` object with the mod's
	 * extended information
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod does not exist or was deleted
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetModInfoAsync", Category = "mod.io|Mods")
	MODIO_API void K2_GetModInfoAsync(FModioModID ModId, FOnGetModInfoDelegate Callback);

	/**
	 * @docpublic
	 * @brief Downloads the logo for the specified mod. Will use existing file if it is already present on disk
	 * @param ModId Mod ID for use in logo retrieval
	 * @param LogoSize Parameter indicating the size of logo that's required
	 * @param Callback Callback providing a status code and an optional path object pointing to the location of the
	 * downloaded image
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod media does not exist or was deleted
	 * @errorcategory `InsufficientSpace`|Not enough space for the file
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetModMediaAsync (Logo)", Category = "mod.io|Mods")
	MODIO_API void K2_GetModMediaLogoAsync(FModioModID ModId, EModioLogoSize LogoSize, FOnGetMediaDelegate Callback);

	/**
	 * @docpublic
	 * @brief Get a gallery image for the specified mod ID. If it already exists on disk the file will be reused unless
	 * it is outdated
	 * @param ModId The mod you want to retrieve an image for
	 * @param GallerySize Size of the image you want to retrieve
	 * @param Index The zero-based index of the image you want to retrieve
	 * @param Callback Callback containing a status code and an Optional containing a path to the image file on disk
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod media does not exist or was deleted
	 * @errorcategory `InsufficientSpace`|Not enough space for the file
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetModMediaAsync (Gallery Image)", Category = "mod.io|Mods")
	MODIO_API void K2_GetModMediaGalleryImageAsync(FModioModID ModId, EModioGallerySize GallerySize, int32 Index,
												   FOnGetMediaDelegate Callback);

	/**
	 * @docpublic
	 * @brief Downloads the creator avatar for a specified mod. Will use existing file if it is already present on disk
	 * and not outdated
	 * @param ModId ID of the mod the creator avatar will be retrieved for
	 * @param AvatarSize Parameter indicating the size of avatar image that's required
	 * @param Callback Callback providing a status code and an optional path object pointing to the location of the
	 * downloaded image
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod media does not exist or was deleted
	 * @errorcategory `InsufficientSpace`|Not enough space for the file
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetModMediaAsync (Avatar)", Category = "mod.io|Mods")
	MODIO_API void K2_GetModMediaAvatarAsync(FModioModID ModId, EModioAvatarSize AvatarSize,
											 FOnGetMediaDelegate Callback);

	/**
	 * @docpublic
	 * @brief Fetches the available tags used on mods for the current game. These tags can them be used in conjunction
	 * with the FilterParams passed to [`ListAllModsAsync`](#listallmodsasync)
	 * Will be cached when first received
	 * @param Callback Callback providing a status code and an optional `ModioModTagOptions` object containing the
	 *available tags
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetModTagOptionsAsync", Category = "mod.io|Mods")
	MODIO_API void K2_GetModTagOptionsAsync(FOnGetModTagOptionsDelegate Callback);

	/**
	 * @docpublic
	 * @brief For a given Mod ID, fetches a list of any mods that the creator has marked as dependencies
	 * @param ModID The mod to retrieve dependencies for
	 * @param Recursive Fetches dependencies recursively up to a depth of 5
	 * @param Callback Callback providing a status code and an optional `ModioModTagOptions` object containing the
	 *available tags
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 * @experimental
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetModDependenciesAsync", Category = "mod.io|Mods")
	MODIO_API void K2_GetModDependenciesAsync(FModioModID ModID, bool Recursive,
											  FOnGetModDependenciesDelegate Callback);

	/**
	 * @brief Adds dependencies to a specified mod, linking it with other mods that are required for it to function
	 * @param ModID The mod to add dependencies to
	 * @param Dependencies List of mod dependencies to add
	 * @param Callback Callback providing a status code to indicate if the dependencies were added successfully
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod could not be found
	 * @error UserDataError::InvalidUser|No authenticated user
	 * @error HttpError::RateLimited|Too many frequent calls to the API. Wait some time and try again.
	 * @error GenericError::BadParameter|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "AddModDependenciesAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_AddModDependenciesAsync(FModioModID ModID, const TArray<FModioModID>& Dependencies,
											  FOnErrorOnlyDelegate Callback);

	/**
	 * @brief Deletes dependencies from a specified mod, unlinking it from other mods that are no longer required.
	 * @param ModID The mod to delete dependencies from
	 * @param Dependencies List of mod IDs to delete as dependencies
	 * @param Callback Callback providing a status code to indicate if the dependencies were deleted successfully
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod could not be found
	 * @error UserDataError::InvalidUser|No authenticated user
	 * @error HttpError::RateLimited|Too many frequent calls to the API. Wait some time and try again.
	 * @error GenericError::BadParameter|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "DeleteModDependenciesAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_DeleteModDependenciesAsync(FModioModID ModID, const TArray<FModioModID>& Dependencies,
												 FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Begins email authentication for the current session by requesting a one-time code be sent to the
	 * specified email address.
	 * @param EmailAddress The email address to send the code to
	 * @param Callback Callback providing a status code indicating the outcome of the request
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserAlreadyAuthenticatedError`|Current user is already authenticated. De-authenticate the current
	 * user with [`ClearUserDataAsync`](#clearuserdataasync), and re-initialize the SDK by calling
	 * [`ShutdownAsync`](#shutdownasync) then [`InitializeAsync`](#initializeasync).
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "RequestEmailAuthCodeAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_RequestEmailAuthCodeAsync(const FModioEmailAddress& EmailAddress, FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Completes email authentication for the current session by submitting the one-time code sent to the user's
	 * email address
	 * @param AuthenticationCode User's authentication code
	 * @param Callback Callback providing a status code indicating if authentication was successful or not
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserAlreadyAuthenticatedError`|Current user is already authenticated. De-authenticate the current
	 * user with [`ClearUserDataAsync`](#clearuserdataasync), and re-initialize the SDK by calling
	 * [`ShutdownAsync`](#shutdownasync) then [`InitializeAsync`](#initializeasync).
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "AuthenticateUserEmailAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_AuthenticateUserEmailAsync(const FModioEmailAuthCode& AuthenticationCode,
												 FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Uses platform-specific authentication to associate a mod.io user account with the current platform user
	 * @param User Authentication payload data to submit to the provider
	 * @param Provider The provider to use to perform the authentication
	 * @param Callback Callback invoked once the authentication request has been made
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `ConfigurationError`|The SDK's configuration is not valid
	 * @errorcategory `InvalidArgsError`|The arguments passed to the function have failed validation
	 * @errorcategory `UserTermsOfUseError`|The user has not yet accepted the mod.io Terms of Use
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserAlreadyAuthenticatedError`|Current user is already authenticated. De-authenticate the current
	 * user with [`ClearUserDataAsync`](#clearuserdataasync), and re-initialize the SDK by calling
	 * [`ShutdownAsync`](#shutdownasync) followed by [`InitializeAsync`](#initializeasync).
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "AuthenticateUserExternalAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_AuthenticateUserExternalAsync(const FModioAuthenticationParams& User,
													EModioAuthenticationProvider Provider,
													FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Queries the server to verify the state of the currently authenticated user if there is one present
	 * @param Callback Callback invoked with the results of the verification process. An empty `ModioErrorCode`
	 * indicates successful verification i.e. the mod.io server was contactable and the user's authentication remains
	 * valid.
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "VerifyUserAuthenticationAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_VerifyUserAuthenticationAsync(FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief This function retrieves the information required for a game to display the mod.io terms of use to a
	 *player who wishes to create a mod.io account
	 * @param Callback Callback invoked with the terms of use data once retrieved from the server
	 * @requires initialized-sdk
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetTermsOfUseAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_GetTermsOfUseAsync(FOnGetTermsOfUseDelegate Callback);

	/**
	 * @docpublic
	 * @brief Set language to get corresponding data from the server
	 * @param Locale Language to set
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SetLanguage", Category = "mod.io")
	MODIO_API void K2_SetLanguage(EModioLanguage Locale);

	/**
	 * @docpublic
	 * @brief Get the currently applied language
	 * @return Current language
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetLanguage", Category = "mod.io")
	MODIO_API EModioLanguage K2_GetLanguage();

	/**
	 * @docpublic
	 * @brief De-authenticates the current mod.io user for the current session, and clears all user-specific data
	 * stored on the current device. Any subscribed mods that are installed but do not have other local users
	 * subscribed will be uninstalled
	 * @param Callback Callback providing a status code indicating the outcome of clearing the user data. Error codes
	 * returned by this function are informative only - it will always succeed.
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ClearUserDataAsync", Category = "mod.io|User")
	MODIO_API void K2_ClearUserDataAsync(FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Downloads the avatar of the currently authenticated user. Will only perform a download if there is no
	 * local cache of the avatar or if that cached copy is out-of-date.
	 * @param AvatarSize Parameter specifying the size of avatar image to download
	 * @param Callback Callback providing a status code for the download and an optional path to the downloaded image
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetUserMediaAsync (Avatar)", Category = "mod.io|User")
	MODIO_API void K2_GetUserMediaAvatarAsync(EModioAvatarSize AvatarSize, FOnGetMediaDelegate Callback);

	/**
	 * @docpublic
	 * @brief Submits a rating for a mod on behalf of the current user. Submit a neutral rating to effectively clear a
	 * rating already submitted by a user. Submitting other values will overwrite any existing rating submitted by this
	 * user.
	 * @param Mod The mod the user is rating
	 * @param Rating The rating the user wishes to submit
	 * @param Callback Callback providing a status code indicating success or failure of the rating submission
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod does not exist or was deleted
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitModRatingAsync", Category = "mod.io|Mods")
	MODIO_API void K2_SubmitModRatingAsync(FModioModID Mod, EModioRating Rating, FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Gets a new mod handle for use with [`SubmitNewModAsync`](#submitnewmodasync).
	 * @returns New handle
	 * @experimental
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetModCreationHandle", Category = "mod.io|Mods|Submission")
	MODIO_API FModioModCreationHandle K2_GetModCreationHandle();

	/**
	 * @docpublic
	 * @brief Requests the creation of a new mod on the server with the specified parameters
	 * @param Handle The `ModioModCreationHandle` for this submission. Once this method invokes your callback indicating
	 * success, the `ModioModCreationHandle` is invalid for the rest of the session. You should request a new one for
	 * the next submission attempt.
	 * @param Params Information about the new mod to create
	 * @param Callback Callback providing a status code and an optional `ModioModID` for the newly created mod
	 * @experimental
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `InvalidArgsError`|Some fields in `Params` did not pass validation
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitNewModAsync", Category = "mod.io|Mods|Submission")
	MODIO_API void K2_SubmitNewModAsync(FModioModCreationHandle Handle, FModioCreateModParams Params,
										FOnSubmitNewModDelegate Callback);

	/**
	 * @docpublic
	 * @brief Queues the upload of a new modfile release for the specified mod using the submitted parameters. This
	 * function takes an `ModioCreateModFileParams` object to specify the path to the root folder of the new modfile.
	 * The plugin will compress the folder's contents into a .zip archive and queue the result for upload. When the
	 * upload completes, a mod management event will be triggered. Note the plugin is also responsible for decompressing
	 * the archive upon its installation at a later point in time.
	 * @param Mod The ID of the mod you are submitting a file for
	 * @param Params Information about the mod file being created, including the root path of the directory that will
	 * be archived
	 * @experimental
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitNewModFileForMod", Category = "mod.io|Mods|Submission")
	MODIO_API void K2_SubmitNewModFileForMod(FModioModID Mod, FModioCreateModFileParams Params);

	/**
	 * @docpublic
	 * @brief Edits the parameters of a mod by updating any fields set in the `Params` object to match the passed-in
	 * values. Fields left empty on the `Params` object will not be updated.
	 * @param Mod The ID of the mod you wish to edit
	 * @param Params Descriptor containing the fields that should be altered.
	 * @param Callback The callback invoked when the changes have been submitted containing an optional updated
	 * `ModioModInfo` object if the edits were performed successfully
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `InvalidArgsError`|Some fields in `Params` did not pass validation
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitModChangesAsync", Category = "mod.io|Mods|Editing")
	MODIO_API void K2_SubmitModChangesAsync(FModioModID Mod, FModioEditModParams Params,
											FOnGetModInfoDelegate Callback);

	/**
	 * @docpublic
	 * @brief Sends a content report to mod.io. When using this function, please inform your users that if they provide
	 * their contact name or details in the `Report` parameter, this data may be shared with the person responsible for
	 * the content being reported. For more information on what data in a report will be shared with whom, please see
	 * [our website's report form](https://mod.io/report).
	 * @param Report Information about the content being reported and a description of the report.
	 * @param Callback Callback providing a status code to indicate successful submission of the report.
	 * @requires initialized-sdk
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `InvalidArgsError`|Required information in the report did not pass validation
	 * @errorcategory `InvalidArgsError`|The mod ID, game ID, or user ID supplied to `Report` is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ReportContentAsync", Category = "mod.io")
	MODIO_API void K2_ReportContentAsync(FModioReportParams Report, FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Archives a mod. This mod will no longer be able to be viewed or retrieved via the SDK, but it will still
	 * exist should you choose to restore it at a later date. Archiving is restricted to team managers and
	 * administrators only. Note that restoration and permanent deletion of a mod is possible only via web interface.
	 * @param Mod The mod to be archived.
	 * @param Callback Callback providing a status code indicating success or failure of archiving the mod.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `InsufficientPermissions`|The authenticated user does not have permission to archive this mod.
	 * This action is restricted to team managers and administrators only.
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `EntityNotFoundError`|Specified mod does not exist or was deleted
	 * @errorcategory `InvalidArgsError`|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ArchiveModAsync", Category = "mod.io|Mods")
	MODIO_API void K2_ArchiveModAsync(FModioModID Mod, FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Mute a user. This will prevent mod.io from returning mods authored by the muted user.
	 * when performing searches.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @param UserID ID of the User to mute
	 * @param Callback Callback providing a status code indicating success or failure of muting the user.
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied user ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "MuteUserAsync", Category = "mod.io|User")
	MODIO_API void K2_MuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Unmute a user. This allows mod.io to display mods authored by the now unmuted user when performing
	 * searches.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @param UserID ID of the user to unmute
	 * @param Callback Callback providing a status code indicating success or failure of unmuting the user.
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 * @errorcategory `InvalidArgsError`|The supplied user ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "UnmuteUserAsync", Category = "mod.io|User")
	MODIO_API void K2_UnmuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief List all the users that have been muted by the current user.
	 * @param Callback Callback providing a status code indicating success or failure of the operation, and an optional
	 * containing a list of muted users if successful.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `UserNotAuthenticatedError`|No authenticated user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetMutedUsersAsync", Category = "mod.io|User")
	MODIO_API void K2_GetMutedUsersAsync(FOnMuteUsersDelegate Callback);

	/**
	 * @docpublic
	 * @brief Provides a list of mods that the user has submitted or is a team member of for the current game,
	 * applying the parameters specified in the filter.
	 * @param Filter Filter to apply when listing the user's created mods.
	 * @param Callback Callback invoked when the call succeeds, or when an error occurs.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory `NetworkError`|Couldn't connect to mod.io servers
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `RateLimited`|Too many frequent calls to the API. Wait some time and try again.
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ListUserCreatedModsAsync", Category = "mod.io|Mods")
	MODIO_API void K2_ListUserCreatedModsAsync(const FModioFilterParams& Filter,
											   FOnListUserCreatedModsDelegate Callback);

	/**
	 * @docpublic
	 * @brief Returns the default mod installation directory for this game and platform, ignoring overrides and without
	 * requiring the SDK to be initialized.
	 * @param GameID The `ModioGameID` of the game we're fetching the default mod installation directory for.
	 * @return The default mod installation directory for the specified game on the current platform
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetDefaultModInstallationDirectory", Category = "mod.io|Mods")
	static MODIO_API FString K2_GetDefaultModInstallationDirectory(FModioGameID GameID);

	/**
	 * @docpublic
	 * @brief Install every temp mod specified by `ModIds` if not already installed.
	 * @param ModIds TArray of `ModioModID`s to install as temp mods
	 * @return Error code indicating the status of the `TempModSet`. Will be empty if it was successful
	 * @requires initialized-sdk
	 * @requires mod-management-enabled
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `ModManagementDisabled`|Mod management not enabled
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "InitTempModSet", Category = "mod.io|Mods")
	MODIO_API FModioErrorCode K2_InitTempModSet(TArray<FModioModID> ModIds);

	/**
	 * @docpublic
	 * @brief  Add mods to a Temp Mod Set.  Every temp mod specified by `ModIds` will be installed if not already
	 * installed.
	 * @param ModIds TArray of `ModioModID`s to install as temp mods
	 * @return Error code indicating the status of the `TempModSet`. Will be empty if it was successful
	 * @requires initialized-sdk
	 * @requires mod-management-enabled
	 * @requires temp-mod-set-initialized
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `ModManagementDisabled`|Mod management not enabled
	 * @errorcategory `TempModSetNotInitialized`|`TempModSet` not initialized
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "AddToTempModSet", Category = "mod.io|Mods")
	MODIO_API FModioErrorCode K2_AddToTempModSet(TArray<FModioModID> ModIds);

	/**
	 * @docpublic
	 * @brief  Remove mods from a temp mod set.  Every temp mod specified by `ModIds` will be uninstalled unless the
	 * user is already subscribed.
	 * @param ModIds TArray of `ModioModID`s to remove as temp mods
	 * @return Error code indicating the status of the `TempModSet`. Will be empty if it was successful
	 * @requires initialized-sdk
	 * @requires mod-management-enabled
	 * @requires temp-mod-set-initialized
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `ModManagementDisabled`|Mod management not enabled
	 * @errorcategory `TempModSetNotInitialized`|`TempModSet` not initialized
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "RemoveFromTempModSet", Category = "mod.io|Mods")
	MODIO_API FModioErrorCode K2_RemoveFromTempModSet(TArray<FModioModID> ModIds);

	/**
	 * @docpublic
	 * @brief  Uninstall every temp mod unless the user is subscribed.
	 * @return Error code indicating the status of the `TempModSet`. Will be empty if it was successful
	 * @requires initialized-sdk
	 * @requires mod-management-enabled
	 * @requires temp-mod-set-initialized
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `ModManagementDisabled`|Mod management not enabled
	 * @errorcategory `TempModSetNotInitialized`|`TempModSet` not initialized
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "CloseTempModSet", Category = "mod.io|Mods")
	MODIO_API FModioErrorCode K2_CloseTempModSet();

	/**
	 * @docpublic
	 * @brief	Query every system and temp mod in `TempModSet`
	 * @return TMap using `ModioModID` as keys and `ModioModCollectionEntry` objects providing information about mods
	 * in `TempModSet`
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "QueryTempModSet", Category = "mod.io|Mods")
	MODIO_API TMap<FModioModID, FModioModCollectionEntry> K2_QueryTempModSet();

	/**
	 * @docpublic
	 * @brief Purchases a mod for the current player
	 * @param ModID ID of the mod to purchase
	 * @param ExpectedPrice The price the user is expected to pay for the mod, generally
	 *[`ModioModInfo.Price`](#modiomodinfo). This ensures that there is consistency between the displayed price and the
	 *price in the backend. If there is a mismatch, the purchase will fail.
	 * @param Callback Callback invoked with purchase information once the purchase is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "PurchaseModAsync", Category = "mod.io|Monetization")
	MODIO_API void K2_PurchaseModAsync(FModioModID ModID, FModioUnsigned64 ExpectedPrice,
									   FOnPurchaseModDelegate Callback);

	/**
	 * @docpublic
	 * @brief Get a user delegation token that can be used for S2S service calls
	 * @param Callback Callback invoked with purchase information once the purchase is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetUserDelegationTokenAsync", Category = "mod.io|Monetization")
	MODIO_API void K2_GetUserDelegationTokenAsync(FOnGetUserDelegationTokenDelegate Callback);

	/**
	 * @docpublic
	 * @brief Gets the users current wallet balance. This will also create a wallet for a user if one does not exist.
	 * You should ensure this is called prior to calling [`PurchaseModAsync`](#purchasemodasync)
	 * purchase will fail.
	 * @param Callback Callback invoked with the users wallet balance
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetUserWalletBalanceAsync", Category = "mod.io|Monetization")
	MODIO_API void K2_GetUserWalletBalanceAsync(FOnGetUserWalletBalanceDelegate Callback);

	/**
	 * @docpublic
	 * @brief Fetches the user's purchases. This populates a runtime cache of purchase information
	 * that can be accessed using [`QueryUserPurchasedMods`](#query-user-purchased-mods).
	 * @param Callback Callback invoked once the call has been completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "FetchUserPurchasesAsync", Category = "mod.io|Monetization")
	MODIO_API void K2_FetchUserPurchasesAsync(FOnFetchUserPurchasesDelegate Callback);

	/**
	 * @docpublic
	 * @brief Requests mod.io refresh the available entitlements for the current user purchased through the portal and
	 * currently authenticated.
	 * @param Params Additional parameters.
	 * @param Callback Callback providing an error code indicating success or failure of the refresh operation.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "RefreshUserEntitlementsAsync", Category = "mod.io|Monetization")
	MODIO_API void K2_RefreshUserEntitlementsAsync(const FModioEntitlementParams& Params,
												   FOnRefreshUserEntitlementsDelegate Callback);

	/**
	 * @docpublic
	 * @brief Start a metrics play session
	 * @param Params `ModioMetricsSessionParams` struct containing information of what and how to start a metrics
	 * session
	 * @param Callback Callback providing an error code indicating success or failure of the session start operation
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `RateLimited`|Too many frequent calls to the API. Wait some time and try again.
	 * @errorcategory `InvalidUser`|No authenticated user
	 * @errorcategory `SessionNotInitialized`|Metrics session has not yet been initialized
	 * @errorcategory `SessionIsActive`|Metrics session is currently active and running
	 * @errorcategory `BadParameter`|One or more values in the Metric Session Parameters are invalid
	 * @premiumfeature Metrics
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "MetricsSessionStartAsync", Category = "mod.io|Metrics")
	MODIO_API void K2_MetricsSessionStartAsync(const FModioMetricsSessionParams& Params, FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Sends a single heartbeat to the mod.io server to indicate a session is still active
	 * @param Callback Callback providing an error code indicating success or failure of the session heartbeat
	 * operation
	 *
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `InvalidUser`|No authenticated user
	 * @errorcategory `SessionNotInitialized`|Metrics session has not yet been initialized
	 * @errorcategory `SessionIsNotActive`|Metrics session is not currently running. Call
	 * [`MetricsSessionStartAsync`](#metricssessionstartasync) before attempting to sending a heartbeat.
	 * @premiumfeature Metrics
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "MetricsSessionSendHeartbeatOnceAsync", Category = "mod.io|Metrics")
	MODIO_API void K2_MetricsSessionSendHeartbeatOnceAsync(FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Sends a constant heartbeat at a given interval to the mod.io server to indicate a session is still active
	 * @param IntervalSeconds The frequency in seconds to send a heartbeat to the mod.io server
	 * @param Callback Callback providing an error code indicating success or failure of the session heartbeat
	 * operation
	 *
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `InvalidUser`|No authenticated user
	 * @errorcategory `SessionNotInitialized`|Metrics session has not yet been initialized
	 * @errorcategory `SessionIsNotActive`|Metrics session is not currently running.
	 * Call [`MetricsSessionStartAsync`](#metricssessionstartasync) before attempting to sending a heartbeat.
	 * @olden Metrics
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "MetricsSessionSendHeartbeatAtIntervalAsync",
			  Category = "mod.io|Metrics")
	MODIO_API void K2_MetricsSessionSendHeartbeatAtIntervalAsync(FModioUnsigned64 IntervalSeconds,
																 FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Ends a metrics play session
	 * @param Callback Callback providing an error code indicating success or failure of the session end operation
	 *
	 * @errorcategory `SDKNotInitialized`|SDK not initialized
	 * @errorcategory `RateLimited`|Too many frequent calls to the API. Wait some time and try again.
	 * @errorcategory `InvalidUser`|No authenticated user
	 * @errorcategory `SessionNotInitialized`|Metrics session has not yet been initialized
	 * @errorcategory `SessionIsNotActive`|Metrics session is not currently running.
	 * Call [`MetricsSessionStartAsync`](#metricssessionstartasync) before attempting to end a session.
	 * @premiumfeature Metrics
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "MetricsSessionEndAsync", Category = "mod.io|Metrics")
	MODIO_API void K2_MetricsSessionEndAsync(FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Fetches storage related information, including total availability and how much is being consumed by mod
	 * installations.
	 * @return An `FModioStorageInfo` structure containing storage information
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "QueryStorageInfo", Category = "mod.io|Storage Info")
	MODIO_API FModioStorageInfo K2_QueryStorageInfo();

#pragma endregion

	/*
	 * @brief Converts a two letter ISO 639-1 language code to the corresponding EModioLanguage value.  Defaults to
	 * English if the specified code is not a valid EModioLanguage value.
	 * @param LanguageCode The two letter ISO 639-1 language code to convert
	 * @deprecated 2024.7 Use UModioSDKLibrary::GetLanguageCodeFromString instead
	 */
	UE_DEPRECATED(5.1, "Deprecated as of 2024.7. Use UModioSDKLibrary::GetLanguageCodeFromString instead")
	MODIO_API EModioLanguage ConvertLanguageCodeToModio(FString LanguageCode);
};
