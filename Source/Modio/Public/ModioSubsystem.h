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

#include "ModioImageCache.h"
#include "Subsystems/EngineSubsystem.h"
#include "Types/ModioAuthenticationParams.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioCreateModFileParams.h"
#include "Types/ModioCreateModParams.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioFilterParams.h"
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
#include "Types/ModioUser.h"
#include "Types/ModioValidationError.h"

#include "ModioSubsystem.generated.h"

// Native versions of the delegates
DECLARE_DELEGATE_OneParam(FOnErrorOnlyDelegateFast, FModioErrorCode);
DECLARE_DELEGATE_OneParam(FOnModManagementDelegateFast, FModioModManagementEvent);
DECLARE_DELEGATE_TwoParams(FOnListAllModsDelegateFast, FModioErrorCode, TOptional<FModioModInfoList>);
DECLARE_DELEGATE_TwoParams(FOnGetModInfoDelegateFast, FModioErrorCode, TOptional<FModioModInfo>);
DECLARE_DELEGATE_TwoParams(FOnGetMediaDelegateFast, FModioErrorCode, TOptional<FModioImageWrapper>);
DECLARE_DELEGATE_TwoParams(FOnGetModTagOptionsDelegateFast, FModioErrorCode, TOptional<FModioModTagOptions>);
DECLARE_DELEGATE_TwoParams(FOnGetTermsOfUseDelegateFast, FModioErrorCode, TOptional<FModioTerms>);
DECLARE_DELEGATE_TwoParams(FOnGetModDependenciesDelegateFast, FModioErrorCode, TOptional<FModioModDependencyList>);
DECLARE_DELEGATE_TwoParams(FOnSubmitNewModDelegateFast, FModioErrorCode, TOptional<FModioModID>);

// Blueprint version of delegates
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnErrorOnlyDelegate, FModioErrorCode, ErrorCode);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnModManagementDelegate, FModioModManagementEvent, Event);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnListAllModsDelegate, FModioErrorCode, ErrorCode, FModioOptionalModInfoList,
								   Result);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetModInfoDelegate, FModioErrorCode, ErrorCode, FModioOptionalModInfo, ModInfo);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetMediaDelegate, FModioErrorCode, ErrorCode, FModioOptionalImage, Path);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetModTagOptionsDelegate, FModioErrorCode, ErrorCode, FModioOptionalModTagOptions,
								   ModTagOptions);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetTermsOfUseDelegate, FModioErrorCode, ErrorCode, FModioOptionalTerms, Terms);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGetModDependenciesDelegate, FModioErrorCode, ErrorCode,
								   FModioOptionalModDependencyList, Dependencies);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnSubmitNewModDelegate, FModioErrorCode, ErrorCode, FModioOptionalModID, NewModID);

/**
 * @brief Thin wrapper around the mod.io SDK. This mostly wraps all the functions available in modio/ModioSDK.h that's
 * the public header of the mod.io SDK. It converts mod.io SDK types to a more unreal friendly types and caches some
 * expensive operations so that conversions don't need to be done multiple times
 */
UCLASS(MinimalAPI)
class UModioSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	/** Begin USubsystem interface */
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;
	/** End USubsystem interface */

	/**
	 * @brief Initializes the SDK for the given user. Loads the state of mods installed on the system as well as the
	 * set of mods the specified user has installed on this device
	 * @param InitializeOptions Parameters to the function packed as a struct where all members needs to be initialized
	 * for the call to succeed
	 * @errorcategory NetworkError|Couldn't connect to the mod.io servers
	 * @errorcategory FilesystemError|Couldn't create the user data or common data folders
	 * @errorcategory ConfigurationError|InitOptions contains an invalid value - inspect ec.value() to determine what
	 * was incorrect
	 * @error GenericError::SDKAlreadyInitialized|SDK already initialized
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
	 * @brief Cancels any running internal operations, frees SDK resources, and invokes any pending callbacks with
	 * Modio::GenericError::OperationCanceled . This function will NOT block while the deinitialization occurs.
	 * @param OnShutdownComplete Callback invoked when the plugin is shut down and calling <<RunPendingHandlers>> is no
	 * longer required
	 * @requires initialized-sdk
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 **/
	MODIO_API void ShutdownAsync(FOnErrorOnlyDelegateFast OnShutdownComplete);

	/**
	 * @brief If the last request to the mod.io servers returned a validation failure, this function returns extended
	 * information describing the fields that failed validation.
	 * @return Collection of Modio::FieldError objects, or empty collection if there was no validation failures
	 * @requires initialized-sdk
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io")
	MODIO_API TArray<FModioValidationError> GetLastValidationError();

	/**
	 * @brief Sends a request to the Mod.io server to add the specified mod to the user's list of subscriptions, and
	 * marks the mod for local installation by the SDK
	 * @param ModToSubscribeTo Mod ID of the mod requiring a subscription.
	 * @param OnSubscribeComplete Callback invoked when the subscription request is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	 * @error UserDataError::InvalidUser|No authenticated user
	 **/
	MODIO_API void SubscribeToModAsync(FModioModID ModToSubscribeTo, FOnErrorOnlyDelegateFast OnSubscribeComplete);

	/**
	 * @brief Sends a request to the Mod.io server to remove the specified mod from the user's list of subscriptions.
	 * If no other local users are subscribed to the specified mod this function will also mark the mod for
	 * uninstallation by the SDK.
	 * @param ModToUnsubscribeFrom Mod ID of the mod requiring unsubscription.
	 * @param OnUnsubscribeComplete Callback invoked when the unsubscription request is completed.
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	 * @error UserDataError::InvalidUser|No authenticated user
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @error UserDataError::InvalidUser|No authenticated user
	 **/
	MODIO_API void FetchExternalUpdatesAsync(FOnErrorOnlyDelegateFast OnFetchDone);

	/**
	 * @brief Enables the automatic management of installed mods on the system based on the user's subscriptions.
	 * @param Callback This callback handler will be invoked with a ModManagementEvent for each mod operation performed
	 * by the SDK
	 * @return Modio::ErrorCode indicating if mod management was enabled successfully
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @error ModManagementError::ModManagementAlreadyEnabled|Mod management was already enabled. The mod management
	 * callback has not been changed
	 */
	MODIO_API void EnableModManagement(FOnModManagementDelegateFast Callback);

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
	 * @brief Fetches the currently authenticated Mod.io user profile if there is one associated with the current
	 * platform user
	 * @return Optional Modio::User object containing profile information
	 **/
	MODIO_API TOptional<FModioUser> QueryUserProfile();

	/**
	 * @brief Fetches all mods installed on the system such that a consuming application can present the information in
	 * a UI in order to free up space by uninstalling mods
	 * @return std::map using Mod IDs as keys and ModCollectionEntry objects providing information about mods installed
	 * on the system regardless of which user installed them
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Mod Management")
	MODIO_API TMap<FModioModID, FModioModCollectionEntry> QuerySystemInstallations();

	/**
	 * @brief Provides a list of mods for the current game, that match the parameters specified in the filter
	 * @param Filter Modio::FilterParams object containing any filters that should be applied to the query
	 * @param Callback Callback invoked with a status code and an optional ModInfoList providing mod profiles
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 */
	MODIO_API void ListAllModsAsync(const FModioFilterParams& Filter, FOnListAllModsDelegateFast Callback);

	/**
	 * @brief Fetches detailed information about the specified mod, including description and file metadata for the
	 * most recent release
	 * @param ModId Mod ID of the mod to fetch data
	 * @param Callback Callback providing a status code and an optional Modio::ModInfo object with the mod's extended
	 * information
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @error FilesystemError::InsufficientSpace|Not enough space for the file
	 **/
	MODIO_API void GetModMediaAsync(FModioModID ModId, EModioLogoSize LogoSize, FOnGetMediaDelegateFast Callback);

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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @error FilesystemError::InsufficientSpace|Not enough space for the file
	 **/
	MODIO_API void GetModMediaAsync(FModioModID ModId, EModioGallerySize GallerySize, int32 Index,
									FOnGetMediaDelegateFast Callback);

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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @error FilesystemError::InsufficientSpace|Not enough space for the file
	 **/
	MODIO_API void GetModMediaAsync(FModioModID ModId, EModioAvatarSize AvatarSize, FOnGetMediaDelegateFast Callback);

	/**
	 * @brief Fetches the available tags used on mods for the current game. These tags can them be used in conjunction
	 * with the FilterParams passed to ListAllMods
	 * Will be cached when first received
	 * @param Callback Callback providing a status code and an optional ModTagOptions object containing the available
	 * tags
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory InvalidArgsError|Some fields in Params did not pass validation
	 * @error UserDataError::InvalidUser|No authenticated user
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
	 * @error UserDataError::InvalidUser|No authenticated user
	 */
	MODIO_API void SubmitNewModFileForMod(FModioModID Mod, FModioCreateModFileParams Params);
	/**
	 * @brief Begins email authentication for the current session by requesting a one-time code be sent to the
	 * specified email address if it is associated with a Mod.io account
	 * @param EmailAddress The email address to send the code to
	 * @param Callback Callback providing a status code indicating the outcome of the request
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @error UserAuthError::AlreadyAuthenticated|Authenticated user already signed-in. Call ClearUserDataAsync to
	 * de-authenticate the old user, then Shutdown() and reinitialize the SDK first.
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @error UserAuthError::AlreadyAuthenticated|Authenticated user already signed-in. Call ClearUserDataAsync to
	 * de-authenticate the old user, then Shutdown() and reinitialize the SDK first.
	 **/
	MODIO_API void AuthenticateUserEmailAsync(const FModioEmailAuthCode& AuthenticationCode,
											  FOnErrorOnlyDelegateFast Callback);

	/**
	 * @brief Uses platform-specific authentication to associate a Mod.io user account with the current platform user
	 * @param User Authentication payload data to submit to the provider. 
	 * @param Provider The provider to use to perform the authentication
	 * @param Callback Callback invoked once the authentication request has been made
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @error UserAuthError::AlreadyAuthenticated|Authenticated user already signed-in. Call ClearUserDataAsync to
	 * de-authenticate the old user, then Shutdown() and reinitialize the SDK first.
	 **/
	MODIO_API void AuthenticateUserExternalAsync(const FModioAuthenticationParams& User,
												 EModioAuthenticationProvider Provider,
												 FOnErrorOnlyDelegateFast Callback);

	/**
	 * @docpublic
	 * @brief This function retrieves the information required for a game to display the mod.io terms of use to a player
	 * who wishes to create a mod.io account
	 * @param Provider The provider to use to perform the authentication
	 * @param Locale The language to display the terms of use in
	 * @param Callback Callback invoked with the terms of use data once retrieved from the server
	 * @requires initialized-sdk
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 **/
	MODIO_API void GetTermsOfUseAsync(EModioAuthenticationProvider Provider, EModioLanguage Locale,
									  FOnGetTermsOfUseDelegateFast Callback);

	/**
	 * @brief De-authenticates the current Mod.io user for the current session, and clears all user-specific data
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

	/**
	 * @brief Submits a rating for a mod on behalf of the current user. Submit a neutral rating to effectively clear a
	 * rating already submitted by a user. Submitting other values will overwrite any existing rating submitted by this
	 * user.
	 * @param Mod The mod the user is rating
	 * @param Rating The rating the user wishes to submit
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod could not be found
	 * @error UserDataError::InvalidUser|No authenticated user
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
	 */
	MODIO_API void ReportContentAsync(FModioReportParams Report, FOnErrorOnlyDelegateFast Callback);

	/** Get our image cache */
	struct FModioImageCache& GetImageCache() const;

	/*
	@brief Archives a mod. This mod will no longer be able to be viewed or retrieved via the SDK, but it will still
	exist should you choose to restore it at a later date. Archiving is restricted to team managers and administrators
	only. Note that restoration and permanent deletion of a mod is possible only via web interface.
	@param Mod The mod to be archived.
	@requires authenticated-user
	@requires initialized-sdk
	@requires no-rate-limiting
	@error ApiError::InsufficientPermission|The authenticated user does not have permission to archive this mod.This
	action is restricted to team managers and administrators only.
	@errorcategory NetworkError|Couldn't connect to mod.io servers
	@error GenericError::SDKNotInitialized|SDK not initialized
	@errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	*/
	MODIO_API void ArchiveModAsync(FModioModID Mod, FOnErrorOnlyDelegateFast Callback);

private:
	TUniquePtr<struct FModioImageCache> ImageCache;

	void InvalidateUserSubscriptionCache();
	void InvalidateUserInstallationCache();

private:
	/** Cached mod tags, they change so infrequently that we cache them until shutdown */
	TOptional<FModioModTagOptions> CachedModTags;

	/** Cached list of user subscriptions */
	TOptional<TMap<FModioModID, FModioModCollectionEntry>> CachedUserSubscriptions;

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
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "InitializeAsync", Category = "mod.io")
	MODIO_API void K2_InitializeAsync(const FModioInitializeOptions& InitializeOptions,
									  FOnErrorOnlyDelegate OnInitComplete);

	/**
	 * @brief Sends a request to the Mod.io server to add the specified mod to the user's list of subscriptions, and
	 * marks the mod for local installation by the SDK
	 * @param ModToSubscribeTo Mod ID of the mod requiring a subscription.
	 * @param OnSubscribeComplete Callback invoked when the subscription request is completed.
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "SubscribeToModAsync", Category = "mod.io|Mods")
	MODIO_API void K2_SubscribeToModAsync(FModioModID ModToSubscribeTo, FOnErrorOnlyDelegate OnSubscribeComplete);

	/**
	 * @brief Cancels any running internal operations, frees SDK resources, and invokes any pending callbacks with
	 * Modio::GenericError::OperationCanceled . This function will NOT block while the deinitialization occurs.
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "ShutdownAsync", Category = "mod.io")
	MODIO_API void K2_ShutdownAsync(FOnErrorOnlyDelegate OnShutdownComplete);

	/**
	 * @brief Sends a request to the Mod.io server to remove the specified mod from the user's list of subscriptions.
	 * If no other local users are subscribed to the specified mod this function will also mark the mod for
	 * uninstallation by the SDK.
	 * @param ModToUnsubscribeFrom Mod ID of the mod requiring unsubscription.
	 * @param OnUnsubscribeComplete Callback invoked when the unsubscription request is completed.
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
	 * @brief Enables the automatic management of installed mods on the system based on the user's subscriptions.
	 * @param Callback This callback handler will be invoked with a ModManagementEvent for each mod operation performed
	 * by the SDK
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "EnableModManagement", Category = "mod.io|Mod Management")
	MODIO_API void K2_EnableModManagement(FOnModManagementDelegate Callback);

	/**
	 * @brief Provides progress information for a mod installation or update operation if one is currently in progress.
	 * @return Optional ModProgressInfo object containing information regarding the progress of the installation
	 * operation.
	 */
	UFUNCTION(BlueprintPure, DisplayName = "QueryCurrentModUpdate", Category = "mod.io|Mod Management")
	MODIO_API FModioOptionalModProgressInfo K2_QueryCurrentModUpdate();

	/**
	 * @brief Forcibly uninstalls a mod from the system. This is intended for use when a host application requires more
	 * room for a mod that the user wants to install, and as such will return an error if the current user is
	 * subscribed to the mod. To remove a mod the current user is subscribed to, use
	 * xref:UnsubscribeFromModAsync[].
	 * @param Callback Callback invoked when the uninstallation is successful, or if it failed because the current user
	 * remains subscribed.
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @error UserDataError::InvalidUser|No authenticated user
	 * @error ModManagementError::AlreadySubscribed|User is still subscribed to the specified mod
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ForceUninstallModAsync", Category = "mod.io|Mod Management")
	MODIO_API void ForceUninstallModAsync(FModioModID ModToRemove, FOnErrorOnlyDelegate Callback);

	/**
	 * @brief Fetches the currently authenticated Mod.io user profile if there is one associated with the current
	 * platform user
	 * @return Optional Modio::User object containing profile information
	 **/
	UFUNCTION(BlueprintPure, DisplayName = "QueryUserProfile", Category = "mod.io|User")
	MODIO_API FModioOptionalUser K2_QueryUserProfile();

	/**
	 * @brief Provides a list of mods for the current game, that match the parameters specified in the filter
	 * @param Filter Modio::FilterParams object containing any filters that should be applied to the query
	 * @param Callback Callback invoked with a status code and an optional ModInfoList providing mod profiles
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ListAllModsAsync", Category = "mod.io|Mods")
	MODIO_API void K2_ListAllModsAsync(const FModioFilterParams& Filter, FOnListAllModsDelegate Callback);

	/**
	 * @brief Fetches detailed information about the specified mod, including description and file metadata for the
	 * most recent release
	 * @param ModId Mod ID of the mod to fetch data
	 * @param Callback Callback providing a status code and an optional Modio::ModInfo object with the mod's extended
	 * information
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @error FilesystemError::InsufficientSpace|Not enough space for the file
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @error FilesystemError::InsufficientSpace|Not enough space for the file
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod media does not exist or was deleted
	 * @error FilesystemError::InsufficientSpace|Not enough space for the file
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @experimental
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetModDependenciesAsync", Category = "mod.io|Mods")
	MODIO_API void K2_GetModDependenciesAsync(FModioModID ModID, FOnGetModDependenciesDelegate Callback);

	/**
	 * @brief Begins email authentication for the current session by requesting a one-time code be sent to the
	 * specified email address if it is associated with a Mod.io account
	 * @param EmailAddress The email address to send the code to
	 * @param Callback Callback providing a status code indicating the outcome of the request
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @error UserAuthError::AlreadyAuthenticated|Authenticated user already signed-in. Call ClearUserDataAsync to
	 * de-authenticate the old user, then Shutdown() and reinitialize the SDK first.
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @error UserAuthError::AlreadyAuthenticated|Authenticated user already signed-in. Call ClearUserDataAsync to
	 * de-authenticate the old user, then Shutdown() and reinitialize the SDK first.
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "AuthenticateUserEmailAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_AuthenticateUserEmailAsync(const FModioEmailAuthCode& AuthenticationCode,
												 FOnErrorOnlyDelegate Callback);

	/**
	 * @brief Uses platform-specific authentication to associate a Mod.io user account with the current platform user
	 * @param User Authentication payload data to submit to the provider
	 * @param Provider The provider to use to perform the authentication
	 * @param Callback Callback invoked once the authentication request has been made
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires no-authenticated-user
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @error UserAuthError::AlreadyAuthenticated|Authenticated user already signed-in. Call ClearUserDataAsync to
	 * de-authenticate the old user, then Shutdown() and reinitialize the SDK first.
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "AuthenticateUserExternalAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_AuthenticateUserExternalAsync(const FModioAuthenticationParams& User,
													EModioAuthenticationProvider Provider,
													FOnErrorOnlyDelegate Callback);

	/**
	 * @brief This function retrieves the information required for a game to display the mod.io terms of use to a player
	 * who wishes to create a mod.io account
	 * @param Provider The provider to use to perform the authentication
	 * @param Locale The language to display the terms of use in
	 * @param Callback Callback invoked with the terms of use data once retrieved from the server
	 * @requires initialized-sdk
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetTermsOfUseAsync", Category = "mod.io|Authentication")
	MODIO_API void K2_GetTermsOfUseAsync(EModioAuthenticationProvider Provider, EModioLanguage Locale,
										 FOnGetTermsOfUseDelegate Callback);

	/**
	 * @brief De-authenticates the current Mod.io user for the current session, and clears all user-specific data
	 * stored on the current device. Any subscribed mods that are installed but do not have other local users
	 * subscribed will be uninstalled
	 * @param Callback Callback providing a status code indicating the outcome of clearing the user data. Error codes
	 * returned by this function are informative only - it will always succeed.
	 * @requires initialized-sdk
	 * @requires no-rate-limiting
	 * @requires authenticated-user
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @error UserDataError::InvalidUser|No authenticated user

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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @error UserDataError::InvalidUser|No authenticated user
	 **/
	UFUNCTION(BlueprintCallable, DisplayName = "GetUserMediaAsync (Avatar)", Category = "mod.io|User")
	MODIO_API void K2_GetUserMediaAvatarAsync(EModioAvatarSize AvatarSize, FOnGetMediaDelegate Callback);

	/**
	 * @brief Submits a rating for a mod on behalf of the current user. Submit a neutral rating to effectively clear a
	 * rating already submitted by a user. Submitting other values will overwrite any existing rating submitted by this
	 * user.
	 * @param Mod The mod the user is rating
	 * @param Rating The rating the user wishes to submit
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory EntityNotFoundError|Specified mod could not be found
	 * @error UserDataError::InvalidUser|No authenticated user
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
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory InvalidArgsError|Some fields in Params did not pass validation
	 * @error UserDataError::InvalidUser|No authenticated user
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
	 * @error UserDataError::InvalidUser|No authenticated user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitNewModFileForMod", Category = "mod.io|Mods|Submission")
	MODIO_API void K2_SubmitNewModFileForMod(FModioModID Mod, FModioCreateModFileParams Params);

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
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ReportContentAsync", Category = "mod.io")
	MODIO_API void K2_ReportContentAsync(FModioReportParams Report, FOnErrorOnlyDelegate Callback);

	/*
	@brief Archives a mod. This mod will no longer be able to be viewed or retrieved via the SDK, but it will still
	exist should you choose to restore it at a later date. Archiving is restricted to team managers and administrators
	only. Note that restoration and permanent deletion of a mod is possible only via web interface.
	@param Mod The mod to be archived.
	@requires authenticated-user
	@requires initialized-sdk
	@requires no-rate-limiting
	@error ApiError::InsufficientPermission|The authenticated user does not have permission to archive this mod.This
	action is restricted to team managers and administrators only.
	@errorcategory NetworkError|Couldn't connect to mod.io servers
	@error GenericError::SDKNotInitialized|SDK not initialized
	@errorcategory EntityNotFoundError|Specified mod does not exist or was deleted
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "ArchiveModAsync", Category = "mod.io|Mods")
	MODIO_API void K2_ArchiveModAsync(FModioModID Mod, FOnErrorOnlyDelegate Callback);

#pragma endregion
};
