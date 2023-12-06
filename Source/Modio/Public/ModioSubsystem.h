/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Containers/Queue.h"
#include "Core/Public/HAL/Runnable.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "ModioImageCache.h"
#include "Subsystems/EngineSubsystem.h"
#include "Types/ModioAuthenticationParams.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioCreateModFileParams.h"
#include "Types/ModioCreateModParams.h"
#include "Types/ModioEditModParams.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioFilterParams.h"
#include "Types/ModioGameInfo.h"
#include "Types/ModioImageWrapper.h"
#include "Types/ModioInitializeOptions.h"
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
DECLARE_DELEGATE_TwoParams(FOnGetMediaDelegateFast, FModioErrorCode, TOptional<FModioImageWrapper>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGetMediaMulticastDelegateFast, FModioErrorCode, TOptional<FModioImageWrapper>);
DECLARE_DELEGATE_TwoParams(FOnGetModTagOptionsDelegateFast, FModioErrorCode, TOptional<FModioModTagOptions>);
DECLARE_DELEGATE_TwoParams(FOnGetTermsOfUseDelegateFast, FModioErrorCode, TOptional<FModioTerms>);
DECLARE_DELEGATE_TwoParams(FOnGetModDependenciesDelegateFast, FModioErrorCode, TOptional<FModioModDependencyList>);
DECLARE_DELEGATE_TwoParams(FOnSubmitNewModDelegateFast, FModioErrorCode, TOptional<FModioModID>);
DECLARE_DELEGATE_TwoParams(FOnMuteUsersDelegateFast, FModioErrorCode, TOptional<FModioUserList>);
DECLARE_DELEGATE_TwoParams(FOnListUserCreatedModsDelegateFast, FModioErrorCode, TOptional<FModioModInfoList>);
DECLARE_DELEGATE_TwoParams(FOnPreviewExternalUpdatesDelegateFast, FModioErrorCode, TOptional<FModioMapPreview>);

DECLARE_DELEGATE_RetVal(EModioLanguage, FGetCurrentLanguageDelegate);

// Blueprint version of delegates

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnErrorOnlyDelegate, FModioErrorCode, ErrorCode);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnModManagementDelegate, FModioModManagementEvent, Event);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnListAllModsDelegate, FModioErrorCode, ErrorCode, FModioOptionalModInfoList,
								   Result);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetModInfoDelegate, FModioErrorCode, ErrorCode, FModioOptionalModInfo, ModInfo);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetGameInfoDelegate, FModioErrorCode, ErrorCode, FModioOptionalGameInfo,
								   GameInfo);

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

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPreviewExternalUpdatesDelegate, FModioErrorCode, ErrorCode, FModioOptionalMapPreview, ModioPreviewMap);

class ARunnableThread;

class FModioBackgroundThread : public FRunnable
{
public:
	bool bStopThread = false;

	FModioBackgroundThread(class UModioSubsystem* modio);
	~FModioBackgroundThread();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	void EndThread();

	class UModioSubsystem* CurrentModio;
	FRunnableThread* CurrentRunningThread = nullptr;
};

/**
 * @brief Thin wrapper around the mod.io SDK. This mostly wraps all the functions available in modio/ModioSDK.h that's
 * the public header of the mod.io SDK. It converts mod.io SDK types to a more unreal friendly types and caches some
 * expensive operations so that conversions don't need to be done multiple times
 */
UCLASS(MinimalAPI)
class UModioSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

protected:
	class FModioBackgroundThread* BackgroundThread = nullptr;

#if WITH_EDITOR
	/// @brief Internal method used for emitting a warning during PIE if the Plugin was initialized during that PIE
	/// session
	void CheckShutdownBeforePIEClose(UWorld*);
	bool bInitializedDuringPIE = false;
#endif

private:
	bool bUseBackgroundThread = false;

public:
	FOnUserProfileUpdatedDelegate OnUserProfileUpdatedDelegate;

	/** Begin USubsystem interface */
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;
	/** End USubsystem interface */

	/// @brief Bind to a function that returns the appropriate EModioLanguage for the game. The Terms of Use will be
	/// displayed in this language during authentication.
	FGetCurrentLanguageDelegate GetCurrentLanguage;

	UFUNCTION(BlueprintCallable, Category = "mod.io")
	MODIO_API bool IsUsingBackgroundThread();

	UFUNCTION(BlueprintCallable, Category = "mod.io")
	MODIO_API void KillBackgroundThread();

	/**
	 * @brief Initializes the SDK for the given user. Loads the state of mods installed on the system as well as the
	 * set of mods the specified user has installed on this device
	 * @param InitializeOptions Parameters to the function packed as a struct where all members needs to be initialized
	 * for the call to succeed
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory FilesystemError|Couldn't create the user data or common data folders
	 * @errorcategory ConfigurationError|InitializeOptions contains an invalid value - inspect ec.value() to determine
	 * what was incorrect
	 * @errorcategory SDKAlreadyInitialized|SDK already initialized
	 */
	MODIO_API void InitializeAsync(const FModioInitializeOptions& InitializeOptions,
								   FOnErrorOnlyDelegateFast OnInitComplete);

	/**
	 * @brief Sets the global logging level - messages with a log level below the specified value will not be displayed
	 * @note This is a transient function, and won't be stored to disk. So at the next engine start, the log level will
	 * be restored to the one in your FModioRuntimeSettings
	 * @param Level Value indicating which priority of messages should be included in the log output
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io")
	MODIO_API void SetLogLevel(EModioLogLevel UnrealLogLevel);

	/**
	 * @brief Runs any pending SDK work on the calling thread, including invoking any callbacks passed to asynchronous
	 * operations.
	 * NOTE: This should be called while xref:InitializeAsync[] and xref:ShutdownAsync[] are running,
		as they both utilize the internal event loop for functionality.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io")
	MODIO_API void RunPendingHandlers();

	/**
	 * @brief Cancels any running internal operations, frees SDK resources, and invokes any pending callbacks with an OperationCanceled error category. This function will NOT block while the deinitialization occurs.
	 * @param OnShutdownComplete Callback invoked when the plugin is shut down and calling <<RunPendingHandlers>> is no
	 * longer required
	 * @requires initialized-sdk
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 **/
	MODIO_API void ShutdownAsync(FOnErrorOnlyDelegateFast OnShutdownComplete);

	/**
	 * @brief If the last request to the mod.io servers returned a validation failure, this function returns extended
	 * information describing the fields that failed validation.
	 * @return Collection of FModioValidationError objects, or empty collection if there were no validation failures
	 * @requires initialized-sdk
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io")
	MODIO_API TArray<FModioValidationError> GetLastValidationError();

	/**
	 * @brief Sends a request to the mod.io server to add the specified mod to the user's list of subscriptions, and
	 * marks the mod for local installation by the SDK
	 * @param ModToSubscribeTo Mod ID of the mod requiring a subscription.
	 * @param OnSubscribeComplete Callback invoked when the subscription request is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	MODIO_API void SubscribeToModAsync(FModioModID ModToSubscribeTo, FOnErrorOnlyDelegateFast OnSubscribeComplete);

	/**
	 * @brief Sends a request to the mod.io server to remove the specified mod from the user's list of subscriptions.
	 * If no other local users are subscribed to the specified mod this function will also mark the mod for
	 * uninstallation by the SDK.
	 * @param ModToUnsubscribeFrom Mod ID of the mod requiring unsubscription.
	 * @param OnUnsubscribeComplete Callback invoked when the unsubscription request is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	MODIO_API void UnsubscribeFromModAsync(FModioModID ModToUnsubscribeFrom,
										   FOnErrorOnlyDelegateFast OnUnsubscribeComplete);

	/**
	 * @brief Synchronises the local list of the current user's subscribed mods with the server. Any mods that have
	 * been externally subscribed will be automatically marked for installation, and mods that have been externally
	 * removed from the user's subscriptions may be uninstalled if no other local users have a current subscription.
	 * @param OnFetchDone Callback invoked when the external state has been retrieved and merged with the local data
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 **/
	MODIO_API void FetchExternalUpdatesAsync(FOnErrorOnlyDelegateFast OnFetchDone);

	/**
	 * @brief Retrieve a list of updates between the users local mod state, and the server-side state. This allows you to
	 * identify which mods will be modified by the next call to <<FetchExternalUpdatesAsync>> in order to perform any content 
	 * management (such as unloading files) that might be required.
	 * @param OnPreviewDone Callback invoked when the external state has been retrieved. It contains a dictionary with ModID
	 * as keys and changes as values. Empty when there are no differences between local and the mod.io API service
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 **/
	MODIO_API void PreviewExternalUpdatesAsync(FOnPreviewExternalUpdatesDelegateFast OnPreviewDone);

	/**
	 * @brief Enables the automatic management of installed mods on the system based on the user's subscriptions.
	 * @param Callback This callback handler will be invoked with a ModManagementEvent for each mod operation performed
	 * by the SDK
	 * @return FModioErrorCode indicating if mod management was enabled successfully
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory ModManagementAlreadyEnabled|Mod management was already enabled. The mod management callback has
	 * not been changed.
	 */
	MODIO_API FModioErrorCode EnableModManagement(FOnModManagementDelegateFast Callback);

	/**
	 * @brief Disables automatic installation or uninstallation of mods based on the user's subscriptions. Allows
	 * currently processing installation to complete; will cancel any pending operations when called.
	 **/
	UFUNCTION(BlueprintCallable, Category = "mod.io|Mod Management")
	MODIO_API void DisableModManagement();

	/**
	 * @brief Checks if the automatic management process is currently installing or removing mods
	 * @return True if automatic management is currently performing an operation
	 **/
	UFUNCTION(BlueprintCallable, Category = "mod.io|Mod Management")
	MODIO_API bool IsModManagementBusy();

	/**
	 * @brief Cancels or suspends the current mod update, installation, or upload, and begins processing a pending
	 * operation for the specified mod ID
	 * @param ModToPrioritize The ID for the mod to prioritize
	 * @return Error code indicating the status of the prioritization request. Will be empty if the prioritization was
	 * successful or if the mod was already being processed
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid or not present in the list of pending operations
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Mod Management")
	MODIO_API FModioErrorCode PrioritizeTransferForMod(FModioModID ModToPrioritize);

	/**
	 * @brief Provides progress information for a mod installation or update operation if one is currently in progress.
	 * @return Optional ModProgressInfo object containing information regarding the progress of the installation
	 * operation.
	 */
	MODIO_API TOptional<FModioModProgressInfo> QueryCurrentModUpdate();

	/**
	 * @brief Fetches the local view of the user's subscribed mods, including mods that are subscribed but not
	 * yet installed
	 * @return TMap<FModioID, FModioModCollectionEntry> providing information about the subscribed
	 * mods
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|User")
	MODIO_API const TMap<FModioModID, FModioModCollectionEntry>& QueryUserSubscriptions();

	/**
	 * @brief Fetches the subset of the user's subscribed mods that are installed and therefore ready for loading
	 * @param bIncludeOutdatedMods Include subscribed mods that are installed but have an updated version on the server
	 * that has not yet been installed
	 * @return TMap<FModioID, FModioModCollectionEntry> providing information about the subscribed
	 * mods
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|User")
	MODIO_API const TMap<FModioModID, FModioModCollectionEntry>& QueryUserInstallations(bool bIncludeOutdatedMods);

	/**
	 * @brief Fetches the currently authenticated mod.io user profile if there is one associated with the current
	 * platform user
	 * @return Optional FModioUser object containing profile information
	 **/
	MODIO_API TOptional<FModioUser> QueryUserProfile();

	/**
	 * @brief Fetches all mods installed on the system such that a consuming application can present the information in
	 * a UI in order to free up space by uninstalling mods
	 * @return TMap using Mod IDs as keys and ModCollectionEntry objects providing information about mods installed
	 * on the system regardless of which user installed them
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Mod Management")
	MODIO_API TMap<FModioModID, FModioModCollectionEntry> QuerySystemInstallations();

	/**
	 * @brief Provides a list of mods for the current game, that match the parameters specified in the filter
	 * @param Filter FModioFilterParams object containing any filters that should be applied to the query
	 * @param Callback Callback invoked with a status code and an optional ModInfoList providing mod profiles
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 */
	MODIO_API void ListAllModsAsync(const FModioFilterParams& Filter, FOnListAllModsDelegateFast Callback);

	/**
	 * @brief Fetches detailed information about the specified game
	 * @param GameID Game ID of the game data to fetch
	 * @param Callback Callback providing a status code and an optional FModioGameInfo object with the game's extended
	 * information
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory EntityNotFoundError|Specified game does not exist or was deleted
	 * @errorcategory InvalidArgsError|The supplied game ID is invalid
	 **/
	MODIO_API void GetGameInfoAsync(FModioGameID GameID, FOnGetGameInfoDelegateFast Callback);

	/**
	 * @brief Fetches detailed information about the specified mod, including description and file metadata for the
	 * most recent release
	 * @param ModId Mod ID of the mod to fetch data
	 * @param Callback Callback providing a status code and an optional FModioModInfo object with the mod's extended
	 * information
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	MODIO_API void GetModInfoAsync(FModioModID ModId, FOnGetModInfoDelegateFast Callback);

	/**
	 * @brief Downloads the logo for the specified mod. Will use existing file if it is already present on disk
	 * @param ModId Mod ID for use in logo retrieval
	 * @param LogoSize Parameter indicating the size of logo that's required
	 * @param Callback Callback providing a status code and an optional path object pointing to the location of the
	 * downloaded image
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @errorcategory InsufficientSpace|Not enough space for the file
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	MODIO_API void GetModMediaAsync(FModioModID ModId, EModioLogoSize LogoSize, FOnGetMediaDelegateFast Callback);

private:
	TMap<TTuple<FModioModID, EModioLogoSize>, FOnGetMediaMulticastDelegateFast> PendingModMediaLogoRequests;

public:
	/**
	 * @brief Get a gallery image for the specified mod ID. If it already exists on disk the file will be reused unless
	 * it is outdated
	 * @param ModId The mod you want to retrieve an image for
	 * @param GallerySize Size of the image you want to retrieve
	 * @param Index The zero-based index of the image you want to retrieve
	 * @param Callback Callback containing a status code and an Optional containing a path to the image file on disk
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @errorcategory InsufficientSpace|Not enough space for the file
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	MODIO_API void GetModMediaAsync(FModioModID ModId, EModioGallerySize GallerySize, int32 Index,
									FOnGetMediaDelegateFast Callback);

private:
	TMap<TTuple<FModioModID, EModioGallerySize, int32>, FOnGetMediaMulticastDelegateFast> PendingModMediaGalleryRequests;

public:
	/**
	 * @brief Downloads the creator avatar for a specified mod. Will use existing file if it is already present on disk
	 * and not outdated
	 * @param ModId ID of the mod the creator avatar will be retrieved for
	 * @param AvatarSize Parameter indicating the size of avatar image that's required
	 * @param Callback Callback providing a status code and an optional path object pointing to the location of the
	 * downloaded image
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @errorcategory InsufficientSpace|Not enough space for the file
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	MODIO_API void GetModMediaAsync(FModioModID ModId, EModioAvatarSize AvatarSize, FOnGetMediaDelegateFast Callback);

private:
	TMap<TTuple<FModioModID, EModioAvatarSize>, FOnGetMediaMulticastDelegateFast> PendingModMediaAvatarRequests;

public:
	/**
	 * @brief Fetches the available tags used on mods for the current game. These tags can them be used in conjunction
	 * with the FilterParams passed to ListAllMods
	 * Will be cached when first received
	 * @param Callback Callback providing a status code and an optional ModTagOptions object containing the available
	 * tags
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 **/
	MODIO_API void GetModTagOptionsAsync(FOnGetModTagOptionsDelegateFast Callback);

	/**
	 * @brief For a given Mod ID, fetches a list of any mods that the creator has marked as dependencies
	 * @param ModID The mod to retrieve dependencies for
	 * @param Callback Callback providing a status code and an optional ModTagOptions object containing the available
	 * tags
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 * @experimental
	 **/
	MODIO_API void GetModDependenciesAsync(FModioModID ModID, FOnGetModDependenciesDelegateFast Callback);

	/**
	 * @brief Gets a new mod handle for use with SubmitNewModAsync.
	 * @returns New handle
	 * @experimental
	 */
	MODIO_API FModioModCreationHandle GetModCreationHandle();

	/**
	 * @brief Requests the creation of a new mod on the server with the specified parameters
	 * @param Handle The ModCreationHandle for this submission. Once this method invokes your callback indicating
	 * success, the ModCreationHandle is invalid for the rest of the session and you should request a new one for the
	 * next submission attempt.
	 * @param Params Information about the new mod to create
	 * @param Callback Callback providing a status code and an optional FModioModID for the newly created mod
	 * @experimental
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory InvalidArgsError|Some fields in Params did not pass validation
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 */
	MODIO_API void SubmitNewModAsync(FModioModCreationHandle Handle, FModioCreateModParams Params,
									 FOnSubmitNewModDelegateFast Callback);

	/**
	 * @brief Queues the upload of a new mod file release for the specified mod, using the submitted parameters. The
	 * upload's progress can be tracked in the same way as downloads; when completed, a Mod Management Event will be
	 * triggered with the result code for the upload.
	 * @param Mod The ID of the mod you are submitting a file for
	 * @param Params Information about the mod file being created, including the root path of the directory that will be
	 * archived
	 * @experimental
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 */
	MODIO_API void SubmitNewModFileForMod(FModioModID Mod, FModioCreateModFileParams Params);

	/**
	 * @brief Edits the parameters of a mod, by updating any fields set in the Params object to match the passed-in
	 * values. Fields left empty on the Params object will not be updated.
	 * @param Mod The ID of the mod you wish to edit
	 * @param Params Descriptor containing the fields that should be altered.
	 * @param Callback The callback invoked when the changes have been submitted, containing an optional updated ModInfo
	 * object if the edits were performed successfully
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory InvalidArgsError|Some fields in Params did not pass validation
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 */
	MODIO_API void SubmitModChangesAsync(FModioModID Mod, FModioEditModParams Params,
										 FOnGetModInfoDelegateFast Callback);

	/**
	 * @brief Begins email authentication for the current session by requesting a one-time code be sent to the
	 * specified email address if it is associated with a mod.io account
	 * @param EmailAddress The email address to send the code to
	 * @param Callback Callback providing a status code indicating the outcome of the request
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 **/
	MODIO_API void RequestEmailAuthCodeAsync(const FModioEmailAddress& EmailAddress, FOnErrorOnlyDelegateFast Callback);

	/**
	 * @brief Completes email authentication for the current session by submitting the one-time code sent to the user's
	 * email address
	 * @param AuthenticationCode User's authentication code
	 * @param Callback Callback providing a status code indicating if authentication was successful or not
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserAlreadyAuthenticatedError|Authenticated user already signed-in. De-authenticate the current
	 * user with ClearUserDataAsync(), and re-initialize the SDK by calling ShutdownAsync() then InitializeAsync().
	 **/
	MODIO_API void AuthenticateUserEmailAsync(const FModioEmailAuthCode& AuthenticationCode,
											  FOnErrorOnlyDelegateFast Callback);

	/**
	 * @brief Uses platform-specific authentication to associate a mod.io user account with the current platform user
	 * @param User Authentication payload data to submit to the provider.
	 * @param Provider The provider to use to perform the authentication
	 * @param Callback Callback invoked once the authentication request has been made
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory ConfigurationError|The SDK's configuration is not valid
	 * @errorcategory InvalidArgsError|The arguments passed to the function have failed validation
	 * @errorcategory UserTermsOfUseError|The user has not yet accepted the mod.io Terms of Use
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserAlreadyAuthenticatedError|Authenticated user already signed-in. De-authenticate the current
	 * user with ClearUserDataAsync(), and re-initialize the SDK by calling ShutdownAsync() then InitializeAsync().
	 **/
	MODIO_API void AuthenticateUserExternalAsync(const FModioAuthenticationParams& User,
												 EModioAuthenticationProvider Provider,
												 FOnErrorOnlyDelegateFast Callback);
	/**
	 * @docpublic
	 * @brief Queries the server to verify the state of the currently authenticated user if there is one present. An
	 * empty ErrorCode is passed to the callback to indicate successful verification, i.e. the mod.io server was
	 * contactable and the user's authentication remains valid.
	 * @param Callback Callback invoked with the results of the verification process
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 */
	MODIO_API void VerifyUserAuthenticationAsync(FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief This function retrieves the information required for a game to display the mod.io terms of use to a player
	 * who wishes to create a mod.io account
	 * @param Provider The provider to use to perform the authentication
	 * @param Locale The language to display the terms of use in
	 * @param Callback Callback invoked with the terms of use data once retrieved from the server
	 * @requires initialized-sdk
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @deprecated Deprecated as of 2023.10. Call GetTermsOfUseAsync() without an EModioAuthenticationProvider instead.
	 **/
	UE_DEPRECATED(
		4.26, "Deprecated as of 2023.10. Call GetTermsOfUseAsync() without an EModioAuthenticationProvider instead.")
	MODIO_API void GetTermsOfUseAsync(EModioAuthenticationProvider Provider, EModioLanguage Locale,
									  FOnGetTermsOfUseDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief This function retrieves the information required for a game to display the mod.io terms of use to a player
	 * who wishes to create a mod.io account
	 * @param Locale The language to display the terms of use in
	 * @param Callback Callback invoked with the terms of use data once retrieved from the server
	 * @requires initialized-sdk
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 **/
	MODIO_API void GetTermsOfUseAsync(EModioLanguage Locale, FOnGetTermsOfUseDelegateFast Callback);

	/**
	 * @brief De-authenticates the current mod.io user for the current session, and clears all user-specific data
	 * stored on the current device. Any subscribed mods that are installed but do not have other local users
	 * subscribed will be uninstalled
	 * @param Callback Callback providing a status code indicating the outcome of clearing the user data. Error codes
	 * returned by this function are informative only - it will always succeed.
	 **/
	MODIO_API void ClearUserDataAsync(FOnErrorOnlyDelegateFast Callback);

	/**
	 * @brief Downloads the avatar of the currently authenticated user. Will only perform a download if there is no
	 * local cache of the avatar or if that cached copy is out-of-date.
	 * @param AvatarSize Parameter specifying the size of avatar image to download
	 * @param Callback Callback providing a status code for the download and an optional path to the downloaded image
	 **/
	MODIO_API void GetUserMediaAsync(EModioAvatarSize AvatarSize, FOnGetMediaDelegateFast Callback);

private:
	TMap<EModioAvatarSize, FOnGetMediaMulticastDelegateFast> PendingUserMediaRequests;

public:
	/**
	 * @brief Submits a rating for a mod on behalf of the current user. Submit a neutral rating to effectively clear a
	 * rating already submitted by a user. Submitting other values will overwrite any existing rating submitted by this
	 * user.
	 * @param Mod The mod the user is rating
	 * @param Rating The rating the user wishes to submit
	 * @param Callback Callback providing a status code indicating success or failure of the rating submission
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 */
	MODIO_API void SubmitModRatingAsync(FModioModID Mod, EModioRating Rating, FOnErrorOnlyDelegateFast Callback);

	/**
	 * @brief Sends a content report to mod.io. When using this function, please inform your users that if they provide
	 * their contact name or details in the Report parameter, that those may be shared with the person responsible for
	 * the content being reported. For more information on what data in a report will be shared with whom, please see
	 * link:https://mod.io/report/widget[our website's report form] for more information.
	 * @param Report Information about the content being reported and a description of the report.
	 * @param Callback Callback providing a status code to indicate successful submission of the report.
	 * @requires initialized-sdk
	 * @errorcategory NetworkError|Couldn't Connect to mod.io servers
	 * @errorcategory InvalidArgsError|Required information in the report did not pass validation
	 * @errorcategory InvalidArgsError|The mod ID, game ID, or user ID supplied to Report is invalid
	 */
	MODIO_API void ReportContentAsync(FModioReportParams Report, FOnErrorOnlyDelegateFast Callback);

	/** Get our image cache */
	struct FModioImageCache& GetImageCache() const;

	/*
	@brief Archives a mod. This mod will no longer be able to be viewed or retrieved via the SDK, but it will still
	exist should you choose to restore it at a later date. Archiving is restricted to team managers and administrators
	only. Note that restoration and permanent deletion of a mod is possible only via web interface.
	@param Mod The mod to be archived.
	@param Callback Callback providing a status code indicating success or failure of archiving the mod.
	@requires authenticated-user
	@requires initialized-sdk
	@requires no-rate-limiting
	@errorcategory InsufficientPermissions|The authenticated user does not have permission to archive this mod. This
	action is restricted to team managers and administrators only.
	@errorcategory NetworkError|Couldn't connect to mod.io servers
	@errorcategory SDKNotInitialized|SDK not initialized
	@errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	@errorcategory InvalidArgsError|The supplied mod ID is invalid
	*/
	MODIO_API void ArchiveModAsync(FModioModID Mod, FOnErrorOnlyDelegateFast Callback);

	/**
	 * @brief Forcibly uninstalls a mod from the system. This can be used when the host application requires additional
	 * space for other mods. The current user must not be subscribed to the mod to force uninstall. To remove a mod the
	 * current user is subscribed to, first use xref:UnsubscribeFromModAsync[]. If the mod does not uninstall (due to a
	 * different user on the same system remaining subscribed), ForceUninstallModAsync can be called next.
	 * @param ModToRemove The mod ID to force uninstall.
	 * @param Callback Callback indicating success or failure of the uninstallation.
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory ApiErrorRefSuccess|User is still subscribed to the specified mod
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 */
	MODIO_API void ForceUninstallModAsync(FModioModID ModToRemove, FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief Mute a user. This will prevent mod.io from returning mods authored by the muted user.
	 * when performing searches.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @param UserID ID of the User to mute
	 * @param Callback Callback providing a status code indicating success or failure of muting the user.
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied user ID is invalid
	 */
	MODIO_API void MuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegateFast Callback);

	/*
	 * @docpublic
	 * @brief Unmute a user. This will allow mod.io to mods authored by the previously muted user.
	 * when performing searches.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @param UserID ID of the User to unmute
	 * @param Callback Callback providing a status code indicating success or failure of unmuting the user.
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied user ID is invalid
	 */
	MODIO_API void UnmuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegateFast Callback);

	/*
	 * @docpublic
	 * @brief List all the users that have been muted by the current user.
	 * @param Callback Callback providing a status code indicating success or failure of the operation, and an Optional
	 * containing a list of muted users if successful.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 */
	MODIO_API void GetMutedUsersAsync(FOnMuteUsersDelegateFast Callback);

	/**
	 * @brief Provides a list of mods that the user has submitted, or is a team member for, for the current game,
	 * applying the parameters specified in the filter.
	 * @param Filter Filter to apply to listing the user's created mods.
	 * @param Callback Callback invoked when the call succeeds, or when an error occurs.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory RateLimited|Too many frequent calls to the API. Wait some time and try again.
	 */
	MODIO_API void ListUserCreatedModsAsync(FModioFilterParams Filter, FOnListUserCreatedModsDelegateFast Callback);

private:
	TUniquePtr<struct FModioImageCache> ImageCache;

	void InvalidateUserSubscriptionCache();
	void InvalidateUserInstallationCache();

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

	/** Cached list of user installation */
	TOptional<TMap<FModioModID, FModioModCollectionEntry>> CachedUserInstallationWithOutdatedMods;
	TOptional<TMap<FModioModID, FModioModCollectionEntry>> CachedUserInstallationWithoutOutdatedMods;

public:
#pragma region Blueprint methods

	/**
	 * @brief Initializes the SDK for the given user. Loads the state of mods installed on the system as well as the
	 * set of mods the specified user has installed on this device
	 * @param InitializeOptions Parameters to the function packed as a struct where all members needs to be initialized
	 * for the call to succeed
	 * @param OnInitComplete Callback which will be invoked with the result of initialization
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory FilesystemError|Couldn't create the user data or common data folders
	 * @errorcategory ConfigurationError|InitializeOptions contains an invalid value - inspect ec.value() to determine
	 * what was incorrect
	 * @errorcategory SDKAlreadyInitialized|SDK already initialized
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "InitializeAsync", Category = "mod.io")
	MODIO_API void K2_InitializeAsync(const FModioInitializeOptions& InitializeOptions,
									  FOnErrorOnlyDelegate OnInitComplete);

	/**
	 * @brief Sends a request to the mod.io server to add the specified mod to the user's list of subscriptions, and
	 * marks the mod for local installation by the SDK
	 * @param ModToSubscribeTo Mod ID of the mod requiring a subscription.
	 * @param OnSubscribeComplete Callback invoked when the subscription request is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "SubscribeToModAsync", Category = "mod.io|Mods")
	MODIO_API void K2_SubscribeToModAsync(FModioModID ModToSubscribeTo, FOnErrorOnlyDelegate OnSubscribeComplete);

	/**
	 * @brief Cancels any running internal operations, frees SDK resources, and invokes any pending callbacks with
	 * an OperationCanceled error category. This function will NOT block while the deinitialization occurs.
	 * @param OnShutdownComplete Callback invoked when the plugin is shut down and calling <<RunPendingHandlers>> is no
	 * longer required
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "ShutdownAsync", Category = "mod.io")
	MODIO_API void K2_ShutdownAsync(FOnErrorOnlyDelegate OnShutdownComplete);

	/**
	 * @brief Sends a request to the mod.io server to remove the specified mod from the user's list of subscriptions.
	 * If no other local users are subscribed to the specified mod this function will also mark the mod for
	 * uninstallation by the SDK.
	 * @param ModToUnsubscribeFrom Mod ID of the mod requiring unsubscription.
	 * @param OnUnsubscribeComplete Callback invoked when the unsubscription request is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "UnsubscribeFromModAsync", Category = "mod.io|Mods")
	MODIO_API void K2_UnsubscribeFromModAsync(FModioModID ModToUnsubscribeFrom,
											  FOnErrorOnlyDelegate OnUnsubscribeComplete);

	/**
	 * @brief Synchronises the local list of the current user's subscribed mods with the server. Any mods that have
	 * been externally subscribed will be automatically marked for installation, and mods that have been externally
	 * removed from the user's subscriptions may be uninstalled if no other local users have a current subscription.
	 * @param OnFetchDone Callback invoked when the external state has been retrieved and merged with the local data
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "FetchExternalUpdatesAsync", Category = "mod.io|Mod Management")
	MODIO_API void K2_FetchExternalUpdatesAsync(FOnErrorOnlyDelegate OnFetchDone);

	/**
	 * @brief Retrieve a list of updates between the users local mod state, and the server-side state. This allows 
	 * you to identify which mods will be modified by the next call to <<FetchExternalUpdatesAsync>> in order to 
	 * perform any content management (such as unloading files) that might be required.
	 * @param OnPreviewDone Callback invoked when the external state has been retrieved. It contains a dictionary with ModID
	 * as keys and changes as values. Empty when there are no differences between local and the mod.io API service
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "PreviewExternalUpdatesAsync", Category = "mod.io|Mod Management")
	MODIO_API void K2_PreviewExternalUpdatesAsync(FOnPreviewExternalUpdatesDelegate OnPreviewDone);

	/**
	 * @brief Enables the automatic management of installed mods on the system based on the user's subscriptions.
	 * @param Callback This callback handler will be invoked with a ModManagementEvent for each mod operation performed
	 * by the SDK
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "EnableModManagement", Category = "mod.io|Mod Management")
	MODIO_API FModioErrorCode K2_EnableModManagement(FOnModManagementDelegate Callback);

	/**
	 * @brief Provides progress information for a mod installation or update operation if one is currently in progress.
	 * @return Optional ModProgressInfo object containing information regarding the progress of the installation
	 * operation.
	 */
	UFUNCTION(BlueprintPure, DisplayName = "QueryCurrentModUpdate", Category = "mod.io|Mod Management")
	MODIO_API FModioOptionalModProgressInfo K2_QueryCurrentModUpdate();

	/**
	 * @brief Forcibly uninstalls a mod from the system. This can be used when the host application requires additional
	 * space for other mods. The current user must not be subscribed to the mod to force uninstall. To remove a mod the
	 * current user is subscribed to, first use xref:UnsubscribeFromModAsync[]. If the mod does not uninstall (due to a
	 * different user on the same system remaining subscribed), ForceUninstallModAsync can be called next.
	 * @param ModToRemove The mod ID to force uninstall.
	 * @param Callback Callback invoked indicating success or failure of the uninstallation.
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory ApiErrorRefSuccess|User is still subscribed to the specified mod
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ForceUninstallModAsync", Category = "mod.io|Mod Management")
	MODIO_API void K2_ForceUninstallModAsync(FModioModID ModToRemove, FOnErrorOnlyDelegate Callback);

	/**
	 * @brief Fetches the currently authenticated mod.io user profile if there is one associated with the current
	 * platform user
	 * @return FModioOptionalUser object containing profile information
	 **/
	UFUNCTION(BlueprintPure, DisplayName = "QueryUserProfile", Category = "mod.io|User")
	MODIO_API FModioOptionalUser K2_QueryUserProfile();

	/**
	 * @brief Provides a list of mods for the current game, that match the parameters specified in the filter
	 * @param Filter FModioFilterParams object containing any filters that should be applied to the query
	 * @param Callback Callback invoked with a status code and an optional ModInfoList providing mod profiles
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ListAllModsAsync", Category = "mod.io|Mods")
	MODIO_API void K2_ListAllModsAsync(const FModioFilterParams& Filter, FOnListAllModsDelegate Callback);

	/**
	 * @brief Fetches detailed information about the specified game
	 * @param GameID Game ID of the game data to fetch
	 * @param Callback Callback providing a status code and an optional FModioGameInfo object with the game's extended
	 * information
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory EntityNotFoundError|Specified game does not exist
	 * @errorcategory InvalidArgsError|The supplied game ID is invalid
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetGameInfoAsync", Category = "mod.io|Game")
	MODIO_API void K2_GetGameInfoAsync(FModioGameID GameID, FOnGetGameInfoDelegate Callback);

	/**
	 * @brief Fetches detailed information about the specified mod, including description and file metadata for the
	 * most recent release
	 * @param ModId Mod ID of the mod to fetch data
	 * @param Callback Callback providing a status code and an optional FModioModInfo object with the mod's
	 * extended information
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetModInfoAsync", Category = "mod.io|Mods")
	MODIO_API void K2_GetModInfoAsync(FModioModID ModId, FOnGetModInfoDelegate Callback);

	/**
	 * @brief Downloads the logo for the specified mod. Will use existing file if it is already present on disk
	 * @param ModId Mod ID for use in logo retrieval
	 * @param LogoSize Parameter indicating the size of logo that's required
	 * @param Callback Callback providing a status code and an optional path object pointing to the location of the
	 * downloaded image
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @errorcategory InsufficientSpace|Not enough space for the file
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetModMediaAsync (Logo)", Category = "mod.io|Mods")
	MODIO_API void K2_GetModMediaLogoAsync(FModioModID ModId, EModioLogoSize LogoSize, FOnGetMediaDelegate Callback);

	/**
	 * @brief Get a gallery image for the specified mod ID. If it already exists on disk the file will be reused unless
	 * it is outdated
	 * @param ModId The mod you want to retrieve an image for
	 * @param GallerySize Size of the image you want to retrieve
	 * @param Index The zero-based index of the image you want to retrieve
	 * @param Callback Callback containing a status code and an Optional containing a path to the image file on disk
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @errorcategory InsufficientSpace|Not enough space for the file
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetModMediaAsync (Gallery Image)", Category = "mod.io|Mods")
	MODIO_API void K2_GetModMediaGalleryImageAsync(FModioModID ModId, EModioGallerySize GallerySize, int32 Index,
												   FOnGetMediaDelegate Callback);

	/**
	 * @brief Downloads the creator avatar for a specified mod. Will use existing file if it is already present on disk
	 * and not outdated
	 * @param ModId ID of the mod the creator avatar will be retrieved for
	 * @param AvatarSize Parameter indicating the size of avatar image that's required
	 * @param Callback Callback providing a status code and an optional path object pointing to the location of the
	 * downloaded image
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @errorcategory InsufficientSpace|Not enough space for the file
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetModMediaAsync (Avatar)", Category = "mod.io|Mods")
	MODIO_API void K2_GetModMediaAvatarAsync(FModioModID ModId, EModioAvatarSize AvatarSize,
											 FOnGetMediaDelegate Callback);

	/**
	 * @brief Fetches the available tags used on mods for the current game. These tags can them be used in conjunction
	 * with the FilterParams passed to ListAllMods
	 * Will be cached when first received
	 * @param Callback Callback providing a status code and an optional ModTagOptions object containing the available
	 * tags
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetModTagOptionsAsync", Category = "mod.io|Mods")
	MODIO_API void K2_GetModTagOptionsAsync(FOnGetModTagOptionsDelegate Callback);

	/**
	 * @brief For a given Mod ID, fetches a list of any mods that the creator has marked as dependencies
	 * @param ModID The mod to retrieve dependencies for
	 * @param Callback Callback providing a status code and an optional ModTagOptions object containing the available
	 * tags
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 * @experimental
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetModDependenciesAsync", Category = "mod.io|Mods")
	MODIO_API void K2_GetModDependenciesAsync(FModioModID ModID, FOnGetModDependenciesDelegate Callback);

	/**
	 * @brief Begins email authentication for the current session by requesting a one-time code be sent to the
	 * specified email address if it is associated with a mod.io account
	 * @param EmailAddress The email address to send the code to
	 * @param Callback Callback providing a status code indicating the outcome of the request
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserAlreadyAuthenticatedError|Authenticated user already signed-in. De-authenticate the current
	 * user with ClearUserDataAsync(), and re-initialize the SDK by calling ShutdownAsync() then InitializeAsync().
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "RequestEmailAuthCodeAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_RequestEmailAuthCodeAsync(const FModioEmailAddress& EmailAddress, FOnErrorOnlyDelegate Callback);

	/**
	 * @brief Completes email authentication for the current session by submitting the one-time code sent to the user's
	 * email address
	 * @param AuthenticationCode User's authentication code
	 * @param Callback Callback providing a status code indicating if authentication was successful or not
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserAlreadyAuthenticatedError|Authenticated user already signed-in. De-authenticate the current
	 * user with ClearUserDataAsync(), and re-initialize the SDK by calling ShutdownAsync() then InitializeAsync().
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "AuthenticateUserEmailAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_AuthenticateUserEmailAsync(const FModioEmailAuthCode& AuthenticationCode,
												 FOnErrorOnlyDelegate Callback);

	/**
	 * @brief Uses platform-specific authentication to associate a mod.io user account with the current platform user
	 * @param User Authentication payload data to submit to the provider
	 * @param Provider The provider to use to perform the authentication
	 * @param Callback Callback invoked once the authentication request has been made
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserAlreadyAuthenticatedError|Authenticated user already signed-in. De-authenticate the current
	 * user with ClearUserDataAsync(), and re-initialize the SDK by calling ShutdownAsync() then InitializeAsync().
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "AuthenticateUserExternalAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_AuthenticateUserExternalAsync(const FModioAuthenticationParams& User,
													EModioAuthenticationProvider Provider,
													FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Queries the server to verify the state of the currently authenticated user if there is one present. An
	 * empty ErrorCode is passed to the callback to indicate successful verification, i.e. the mod.io server was
	 * contactable and the user's authentication remains valid.
	 * @param Callback Callback invoked with the results of the verification process
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "VerifyUserAuthenticationAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_VerifyUserAuthenticationAsync(FOnErrorOnlyDelegate Callback);

	/**
	 * @brief This function retrieves the information required for a game to display the mod.io terms of use to a player
	 * who wishes to create a mod.io account
	 * @param Provider The provider to use to perform the authentication
	 * @param Locale The language to display the terms of use in
	 * @param Callback Callback invoked with the terms of use data once retrieved from the server
	 * @requires initialized-sdk
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @deprecated Deprecated as of 2023.10. Call K2_GetTermsOfUseAsync() without an EModioAuthenticationProvider
	 *instead
	 **/
	// clang-format off
	UFUNCTION(BlueprintCallable, DisplayName = "GetTermsOfUseAsync", Category = "mod.io|Authentication",
			  meta = (DeprecatedFunction, DeprecationMessage = "Deprecated as of 2023.10. Call K2_GetTermsOfUseAsync() without an EModioAuthenticationProvider instead."))
	MODIO_API void K2_GetTermsOfUseAsync_DEPRECATED(EModioAuthenticationProvider Provider, EModioLanguage Locale,
													FOnGetTermsOfUseDelegate Callback);
	// clang-format on

	/**
	 * @brief This function retrieves the information required for a game to display the mod.io terms of use to a player
	 * who wishes to create a mod.io account
	 * @param Locale The language to display the terms of use in
	 * @param Callback Callback invoked with the terms of use data once retrieved from the server
	 * @requires initialized-sdk
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetTermsOfUseAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_GetTermsOfUseAsync(EModioLanguage Locale, FOnGetTermsOfUseDelegate Callback);

	/**
	 * @brief De-authenticates the current mod.io user for the current session, and clears all user-specific data
	 * stored on the current device. Any subscribed mods that are installed but do not have other local users
	 * subscribed will be uninstalled
	 * @param Callback Callback providing a status code indicating the outcome of clearing the user data. Error codes
	 * returned by this function are informative only - it will always succeed.
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "ClearUserDataAsync", Category = "mod.io|User")
	MODIO_API void K2_ClearUserDataAsync(FOnErrorOnlyDelegate Callback);

	/**
	 * @brief Downloads the avatar of the currently authenticated user. Will only perform a download if there is no
	 * local cache of the avatar or if that cached copy is out-of-date.
	 * @param AvatarSize Parameter specifying the size of avatar image to download
	 * @param Callback Callback providing a status code for the download and an optional path to the downloaded image
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetUserMediaAsync (Avatar)", Category = "mod.io|User")
	MODIO_API void K2_GetUserMediaAvatarAsync(EModioAvatarSize AvatarSize, FOnGetMediaDelegate Callback);

	/**
	 * @brief Submits a rating for a mod on behalf of the current user. Submit a neutral rating to effectively clear a
	 * rating already submitted by a user. Submitting other values will overwrite any existing rating submitted by this
	 * user.
	 * @param Mod The mod the user is rating
	 * @param Rating The rating the user wishes to submit
	 * @param Callback Callback providing a status code indicating success or failure of the rating submission
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitModRatingAsync", Category = "mod.io|Mods")
	MODIO_API void K2_SubmitModRatingAsync(FModioModID Mod, EModioRating Rating, FOnErrorOnlyDelegate Callback);

	/**
	 * @brief Gets a new mod handle for use with SubmitNewModAsync.
	 * @returns New handle
	 * @experimental
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetModCreationHandle", Category = "mod.io|Mods|Submission")
	MODIO_API FModioModCreationHandle K2_GetModCreationHandle();

	/**
	 * @brief Requests the creation of a new mod on the server with the specified parameters
	 * @param Handle The ModCreationHandle for this submission. Once this method invokes your callback indicating
	 * success, the ModCreationHandle is invalid for the rest of the session and you should request a new one for the
	 * next submission attempt.
	 * @param Params Information about the new mod to create
	 * @param Callback Callback providing a status code and an optional FModioModID for the newly created mod
	 * @experimental
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory InvalidArgsError|Some fields in Params did not pass validation
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitNewModAsync", Category = "mod.io|Mods|Submission")
	MODIO_API void K2_SubmitNewModAsync(FModioModCreationHandle Handle, FModioCreateModParams Params,
										FOnSubmitNewModDelegate Callback);

	/**
	 * @brief Queues the upload of a new mod file release for the specified mod, using the submitted parameters. The
	 * upload's progress can be tracked in the same way as downloads; when completed, a Mod Management Event will be
	 * triggered with the result code for the upload.
	 * @param Mod The ID of the mod you are submitting a file for
	 * @param Params Information about the mod file being created, including the root path of the directory that will be
	 * archived
	 * @experimental
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitNewModFileForMod", Category = "mod.io|Mods|Submission")
	MODIO_API void K2_SubmitNewModFileForMod(FModioModID Mod, FModioCreateModFileParams Params);

	/**
	 * @brief Edits the parameters of a mod, by updating any fields set in the Params object to match the passed-in
	 * values. Fields left empty on the Params object will not be updated.
	 * @param Mod The ID of the mod you wish to edit
	 * @param Params Descriptor containing the fields that should be altered.
	 * @param Callback The callback invoked when the changes have been submitted, containing an optional updated ModInfo
	 * object if the edits were performed successfully
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory InvalidArgsError|Some fields in Params did not pass validation
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitModChangesAsync", Category = "mod.io|Mods|Editing")
	MODIO_API void K2_SubmitModChangesAsync(FModioModID Mod, FModioEditModParams Params,
											FOnGetModInfoDelegate Callback);

	/**
	 * @brief Sends a content report to mod.io. When using this function, please inform your users that if they provide
	 * their contact name or details in the Report parameter, that those may be shared with the person responsible for
	 * the content being reported. For more information on what data in a report will be shared with whom, please see
	 * link:https://mod.io/report/widget[our website's report form] for more information.
	 * @param Report Information about the content being reported and a description of the report.
	 * @param Callback Callback providing a status code to indicate successful submission of the report.
	 * @requires initialized-sdk
	 * @errorcategory NetworkError|Couldn't Connect to mod.io servers
	 * @errorcategory InvalidArgsError|Required information in the report did not pass validation
	 * @errorcategory InvalidArgsError|The mod ID, game ID, or user ID supplied to Report is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ReportContentAsync", Category = "mod.io")
	MODIO_API void K2_ReportContentAsync(FModioReportParams Report, FOnErrorOnlyDelegate Callback);

	/*
	 * @brief Archives a mod. This mod will no longer be able to be viewed or retrieved via the SDK, but it will still
	 * exist should you choose to restore it at a later date. Archiving is restricted to team managers and
	 * administrators only. Note that restoration and permanent deletion of a mod is possible only via web interface.
	 * @param Mod The mod to be archived.
	 * @param Callback Callback providing a status code indicating success or failure of archiving the mod.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory InsufficientPermissions|The authenticated user does not have permission to archive this mod. This
	 * action is restricted to team managers and administrators only.
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	 * @errorcategory InvalidArgsError|The supplied mod ID is invalid
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
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied user ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "MuteUserAsync", Category = "mod.io|User")
	MODIO_API void K2_MuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Unmute a user. This will allow mod.io to mods authored by the previously muted user.
	 * when performing searches.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @param UserID ID of the User to unmute
	 * @param Callback Callback providing a status code indicating success or failure of unmuting the user.
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 * @errorcategory InvalidArgsError|The supplied user ID is invalid
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "UnmuteUserAsync", Category = "mod.io|User")
	MODIO_API void K2_UnmuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief List all the users that have been muted by the current user.
	 * @param Callback Callback providing a status code indicating success or failure of the operation, and an Optional
	 * containing a list of muted users if successful.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory UserNotAuthenticatedError|No authenticated user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "GetMutedUsersAsync", Category = "mod.io|User")
	MODIO_API void K2_GetMutedUsersAsync(FOnMuteUsersDelegate Callback);

	/**
	 * @brief Provides a list of mods that the user has submitted, or is a team member for, for the current game,
	 * applying the parameters specified in the filter.
	 * @param Filter Filter to apply to listing the user's created mods.
	 * @param Callback Callback invoked when the call succeeds, or when an error occurs.
	 * @requires authenticated-user
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @errorcategory SDKNotInitialized|SDK not initialized
	 * @errorcategory RateLimited|Too many frequent calls to the API. Wait some time and try again.
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ListUserCreatedModsAsync", Category = "mod.io|Mods")
	MODIO_API void K2_ListUserCreatedModsAsync(const FModioFilterParams& Filter,
											   FOnListUserCreatedModsDelegate Callback);

#pragma endregion

	/*
	 * @brief Converts a two letter ISO 639-1 language code to the corresponding EModioLanguage value.  Defaults to
	 * English if the specified code is not a valid EModioLanguage value.
	 * @param LanguageCode The two letter ISO 639-1 language code to convert
	 */
	MODIO_API EModioLanguage ConvertLanguageCodeToModio(FString LanguageCode);
};