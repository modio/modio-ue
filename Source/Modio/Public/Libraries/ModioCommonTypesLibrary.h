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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioAuthenticationParams.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioInitializeOptions.h"

#include "ModioCommonTypesLibrary.generated.h"

/**
 * @docpublic
 * @brief Utility functions for working with mod.io types
 */
UCLASS()
class MODIO_API UModioCommonTypesLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @docpublic
	 * @brief Create a game id from a integer. Should only be used in conjunction with
	 * [`InitializeAsync`](#initializeasync)
	 * @param GameId A positive integer that maps to your game
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioGameID MakeGameId(int64 GameId);

	/**
	 * @docpublic
	 * @brief Converts a game id to a string for debugging purposes
	 * @param GameId Input Game Id
	 * @return Empty if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (ModioGameId)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_GameIDToString(FModioGameID GameId);

	/**
	 * @docpublic
	 * @brief Creates a `ModioAuthenticationParams` object
	 * @param AuthToken Authentication provider-supplied OAuth token
	 * @param EmailAddress User email address, can be left blank
	 * @param bHasAcceptedTOS Has the user been shown the Terms of Service and accepted the Terms of Service?
	 * @return The constructed `ModioAuthenticationParams` object for use with
	 * [`AuthenticateUserExternalAsync`](#authenticateuserexternalasync)
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioAuthenticationParams MakeAuthParams(const FString AuthToken, const FString EmailAddress,
													 const bool bHasAcceptedTOS);

	/**
	 * @docpublic
	 * @brief Create an ApiKey id from a string. Should only be used in conjunction with
	 * [`InitializeAsync`](#initializeasync)
	 * @param ApiKey The api key from your settings panel on mod.io
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioApiKey MakeApiKey(const FString ApiKey);

	/**
	 * @docpublic
	 * @brief Converts a `ModioApiKey` to a string for debugging purposes
	 * @param ApiKey Input Api Key
	 * @return Empty if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (ModioApiKey)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_ApiKeyToString(FModioApiKey ApiKey);

	/**
	 * @docpublic
	 * @brief Converts a `ModioModID` to a string for debugging purposes
	 * @param ModID Input Mod ID
	 * @return Empty if invalid
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (ModioModID)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_ModIDToString(FModioModID ModID);

	/**
	 * @docpublic
	 * @brief Converts an `FileMetadataID` to a string for debugging purposes
	 * @param FileMetadataID Input File metadata ID
	 * @return Empty if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (FileMetadataID)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_FileMetadataIDToString(FModioFileMetadataID FileMetadataID);

	/**
	 * @docpublic
	 * @brief Converts a `ModioUserID` to a string for debugging purposes
	 * @param UserID User ID to convert
	 * @return Empty if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (ModioUserID)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_UserIDToString(FModioUserID UserID);

	/**
	 * @docpublic
	 * @brief Converts a `ModioEmailAddress` string for debugging purposes
	 * @param EmailAddress Email address to convert
	 * @return Empty if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (ModioEmailAddress)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_EmailAddressToString(FModioEmailAddress EmailAddress);

	/**
	 * @docpublic
	 * @brief Converts a `ModioEmailAuthCode` string for debugging purposes
	 * @param EmailAuthCode Email auth code to convert
	 * @return Empty if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (ModioEmailAuthCode)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_EmailAuthCodeToString(FModioEmailAuthCode EmailAuthCode);

	/**
	 * @docpublic
	 * @brief Make an email address from a string
	 * @param Email Input string to convert
	 * @return FModioEmailAddress object
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToEmailAddress (String)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities")
	static FModioEmailAddress Conv_StringToEmailAddress(const FString& Email);

	/**
	 * @docpublic
	 * @brief Make an email auth code from a string
	 * @param AuthCode Input string to convert
	 * @return Auth code for submission
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToEmailAuthCode (String)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities")
	static FModioEmailAuthCode Conv_StringToEmailAuthCode(const FString& AuthCode);

	/**
	 * @docpublic
	 * @brief Make [`ModioInitializeOptions`](#modioinitializeoptions). Should only be used in conjunction with
	 * [`InitializeAsync`](#initializeasync).
	 * @param GameId - A positive integer that maps to your game.  This can be found in the admin section of your game's
	 * page at https://mod.io/
	 * @param APIKey - The API key for your game.  This can be found in the admin section of your game's page at
	 * https://mod.io/
	 * @param GameEnvironment - The environment your game has been set up on: test or live.
	 * @param PortalInUse The [`EModioPortal`](#EModioPortal) representing the store or service your game is being
	 * @param bUseBackgroundThread - Whether to run the mod.io SDK on a background thread. Defaults to `true`.
	 * distributed through.  Defaults to `EModioPortal::None`.
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioInitializeOptions MakeInitializeOptions(int64 GameId, const FString& APIKey,
	                                                     EModioEnvironment GameEnvironment,
	                                                     EModioPortal PortalInUse = EModioPortal::None, bool bUseBackgroundThread = true);

	/**
	 * @docpublic
	 * @brief Changes the game id for the provided set of [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param Options - The template [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param GameId - The new game id to use
	 * @return New [`ModioInitializeOptions`](#modioinitializeoptions) object with the game id set to the desired value
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static FModioInitializeOptions SetGameId(const FModioInitializeOptions& Options, int64 GameId);

	/**
	 * @docpublic
	 * @brief Changes the API key for the provided set of [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param Options - The template [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param APIKey - The new API key to use
	 * @return New [`ModioInitializeOptions`](#modioinitializeoptions) object with the API key set to the desired value
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (DisplayName = "Set API Key"))
	static FModioInitializeOptions SetAPIKey(const FModioInitializeOptions& Options, const FString& APIKey);

	/**
	 * @docpublic
	 * @brief Changes the game environment for the provided set of [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param Options - The template [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param GameEnvironment - The new environment to use
	 * @return New [`ModioInitializeOptions`](#modioinitializeoptions) object with the game environment set to the
	 * desired value
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static FModioInitializeOptions SetGameEnvironment(const FModioInitializeOptions& Options,
													  EModioEnvironment GameEnvironment);

	/**
	 * @docpublic
	 * @brief Changes the portal for the provided set of [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param Options - The template [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param PortalToUse - The new portal to use
	 * @return New [`ModioInitializeOptions`](#modioinitializeoptions) object with the portal set to the desired value
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static FModioInitializeOptions SetPortal(const FModioInitializeOptions& Options, EModioPortal PortalToUse);

	/**
	 * @docpublic
	 * @brief Changes the background thread usage for the provided set of [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param Options - The template [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param bUseBackgroundThread - Whether to use a background thread
	 * @return New [`ModioInitializeOptions`](#modioinitializeoptions) object with the background thread usage set to the
	 * desired value
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static FModioInitializeOptions SetBackgroundThread(const FModioInitializeOptions& Options, bool bUseBackgroundThread);

	/**
	 * @docpublic
	 * @brief Changes the session identifier for the provided set of [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param Options - The template [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param SessionIdentifier - The new session id to use
	 * @return New [`ModioInitializeOptions`](#modioinitializeoptions) object with the session identifier set to the
	 * desired value
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static FModioInitializeOptions SetSessionIdentifier(const FModioInitializeOptions& Options,
														const FString& SessionIdentifier);

	/**
	 * @docpublic
	 * @brief Sets extended initialization parameters for the provided set of
	 * [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param Options - The template [`ModioInitializeOptions`](#modioinitializeoptions)
	 * @param ExtendedParameters - The new extended parameters to use (will overwrite existing values)
	 * @return New [`ModioInitializeOptions`](#modioinitializeoptions) object with the extended parameters set to the
	 * desired value
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static FModioInitializeOptions SetExtendedInitializationParameters(
		const FModioInitializeOptions& Options, const TMap<FString, FString>& ExtendedParameters);

	/**
	 * @docpublic
	 * @brief Retrieves the raw underlying value from an `ModioModID`. `ModioModID`s are intended as opaque types, so
	 * use with care.
	 * @param In The `ModioModID` to retrieve the value for
	 * @return The underlying value
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Utilities")
	static int64 GetRawValueFromModID(const FModioModID& In);
	
	/**
	 * @docpublic
	 * @brief Compares two mod IDs to check whether they're equal
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Utilities",
			  meta = (CompactNodeTitle = "==", Keywords = "== equal", DisplayName = "ModioModID == ModioModID"))
	static bool EqualTo(const FModioModID& A, const FModioModID& B);

	/**
	 * @docpublic
	 * @brief Compares two mod IDs to check whether they're not equal
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Utilities",
			  meta = (CompactNodeTitle = "!=", Keywords = "!= not equal", DisplayName = "ModioModID != ModioModID"))
	static bool NotEqualTo(const FModioModID& A, const FModioModID& B);

	/**
	 * @docpublic
	 * @brief Create entitlement parameters
	 * @param ExtendedParameters A map to store extended parameters required by some portals
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioEntitlementParams MakeEntitlementParams(const TMap<FString, FString>& ExtendedParameters);

	/**
	 * @docpublic
	 * @brief Create Metrics Session parameters
	 * @param Ids The list of mods to store within the Metrics Session parameters
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioMetricsSessionParams MakeMetricsSessionParams(const TArray<FModioModID>& Ids);

	/**
	 * @docpublic
	 * @brief Create a Guid from a string
	 * @param Guid A string to wrap within a Guid struct
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioGuid MakeGuid(const FString Guid);

	/**
	 * @docpublic
	 * @brief Changes the session identifier for the provided Metrics Session parameters
	 * @param Params The template Metrics Session parameters
	 * @param Id The intended Guid to store in the Metrics Session parameters
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioMetricsSessionParams SetSessionId(const FModioMetricsSessionParams& Params, const FModioGuid& Id);
};