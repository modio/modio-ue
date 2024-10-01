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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioAuthenticationParams.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioInitializeOptions.h"

#include "ModioCommonTypesLibrary.generated.h"

UCLASS()
class MODIO_API UModioCommonTypesLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief Create a game id from a integer, should only be used in conjunction with InitializeAsync
	 * @param GameId - a positive integer that maps to your game
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioGameID MakeGameId(int64 GameId);

	/**
	 * @brief Converts a game id to a string so you can debug output it
	 * @param GameId - Input Game Id
	 * @return InvalidGameId if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (ModioGameId)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_GameIDToString(FModioGameID GameId);

	/**
	 * @brief Creates an AuthenticationParams object
	 * @param AuthToken - Authentication provider-supplied OAuth token
	 * @param EmailAddress - User email address, can be left blank
	 * @param bHasAcceptedTOS - Has the user been shown the TOS and accepted the TOS?
	 * @return The constructed FModioAuthenticationParams object for use with <<K2_AuthenticateUserExternalAsync>>
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioAuthenticationParams MakeAuthParams(const FString AuthToken, const FString EmailAddress,
													 const bool bHasAcceptedTOS);

	/**
	 * @brief Create a ApiKey id from a string, should only be used in conjunction with InitializeAsync
	 * @param ApiKey - a api key from your settings panel on mod.io
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioApiKey MakeApiKey(const FString ApiKey);

	/**
	 * @brief Converts a ApiKey string so you can debug output it
	 * @param ApiKey Input Api Key
	 * @return InvalidApiKey if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (ModioApiKey)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_ApiKeyToString(FModioApiKey ApiKey);

	/**
	 * @brief Converts a ModId string so you can debug output it
	 * @param ModID Input Mod ID
	 * @return InvalidModID if invalid
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (ModioModID)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_ModIDToString(FModioModID ModID);

	/**
	 * @brief Converts a FFileMetadataID to string so you can debug output it
	 * @param FileMetadataID Input File metadata ID
	 * @return InvalidFileMetadataID if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (FileMetadataID)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_FileMetadataIDToString(FModioFileMetadataID FileMetadataID);

	/**
	 * @brief Converts a UserID string so you can debug output it
	 * @param UserID -
	 * @return InvalidUserID if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (ModioUserID)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_UserIDToString(FModioUserID UserID);

	/**
	 * @brief Converts a EmailAddress string so you can debug output it
	 * @param EmailAddress -
	 * @return InvalidEmailAddress if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (ModioEmailAddress)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_EmailAddressToString(FModioEmailAddress EmailAddress);

	/**
	 * @brief Converts a EmailAuthCode string so you can debug output it
	 * @param EmailAuthCode -
	 * @return InvalidEmailAuthCode if invalid
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToString (ModioEmailAuthCode)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities|String")
	static FString Conv_EmailAuthCodeToString(FModioEmailAuthCode EmailAuthCode);

	/**
	 * @brief Make a email address from a string
	 * @param Email Input string to convert
	 * @return FModioEmailAddress object
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToEmailAddress (String)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities")
	static FModioEmailAddress Conv_StringToEmailAddress(const FString& Email);

	/**
	 * @brief Make a email auth code from a string
	 * @param GameId Input string
	 * @return Auth code for submission
	 */
	UFUNCTION(BlueprintPure,
			  meta = (DisplayName = "ToEmailAuthCode (String)", CompactNodeTitle = "->", BlueprintAutocast),
			  Category = "mod.io|Utilities")
	static FModioEmailAuthCode Conv_StringToEmailAuthCode(const FString& AuthCode);

	/**
	 * @brief Make initialization options, should only be used in conjunction with InitializeAsync
	 * @param GameId - a positive integer that maps to your game
	 * @param APIKey - APIKey available at https://<your-game-name>.test.mod.io/edit/api or
	 * https://<your-game-name>.mod.io/edit/api
	 * @param GameEnvironment - If your environment is setup on test or production
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioInitializeOptions MakeInitializeOptions(int64 GameId, const FString& APIKey,
														 EModioEnvironment GameEnvironment,
														 EModioPortal PortalInUse = EModioPortal::None);

	/**
	 * @brief Changes the game id for the provided set of initialization options
	 * @param Options - The template initialization options
	 * @param GameId - The new game id to use
	 * @return New Initialization Options object with the game id set to the desired value
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static FModioInitializeOptions SetGameId(const FModioInitializeOptions& Options, int64 GameId);

	/**
	 * @brief Changes the API key for the provided set of initialization options
	 * @param Options - The template initialization options
	 * @param APIKey - The new API key to use
	 * @return New Initialization Options object with the API key set to the desired value
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (DisplayName = "Set API Key"))
	static FModioInitializeOptions SetAPIKey(const FModioInitializeOptions& Options, const FString& APIKey);

	/**
	 * @brief Changes the game environment for the provided set of initialization options
	 * @param Options - The template initialization options
	 * @param GameEnvironment - The new environment to use
	 * @return New Initialization Options object with the game environment set to the desired value
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static FModioInitializeOptions SetGameEnvironment(const FModioInitializeOptions& Options,
													  EModioEnvironment GameEnvironment);

	/**
	 * @brief Changes the portal for the provided set of initialization options
	 * @param Options - The template initialization options
	 * @param PortalToUse - The new portal to use
	 * @return New Initialization Options object with the portal set to the desired value
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static FModioInitializeOptions SetPortal(const FModioInitializeOptions& Options, EModioPortal PortalToUse);

	/**
	 * @brief Changes the session identifier for the provided set of initialization options
	 * @param Options - The template initialization options
	 * @param SessionIdentifier - The new session id to use
	 * @return New Initialization Options object with the session identifier set to the desired value
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities")
	static FModioInitializeOptions SetSessionIdentifier(const FModioInitializeOptions& Options,
														const FString& SessionIdentifier);

	/**
	 * @brief Sets extended initialization parameters for the provided set of initialization options
	 * @param Options - The template initialization options
	 * @param ExtendedParameters - The new extended parameters to use (will overwrite existing values)
	 * @return New Initialization Options object with the extended parameters set to the desired value
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities")
	static FModioInitializeOptions SetExtendedInitializationParameters(
		const FModioInitializeOptions& Options, const TMap<FString, FString>& ExtendedParameters);

	/**
	 * @brief Retrieves the raw underlying value from a FModioModID. FModioModIDs are intended as opaque types, so use
	 * with care.
	 * @param In - the FModioModID to retrieve the value for
	 * @return The underlying value
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Utilities")
	static int64 GetRawValueFromModID(const FModioModID& In);
	/**
	 * @brief Compares two ModioModIDs, returning true if equal
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Utilities",
			  meta = (CompactNodeTitle = "==", Keywords = "== equal", DisplayName = "ModioModID == ModioModID"))
	static bool EqualTo(const FModioModID& A, const FModioModID& B);

	/**
	 * @brief Compares two ModioModIDs, returning true if not equal
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "mod.io|Utilities",
			  meta = (CompactNodeTitle = "!=", Keywords = "!= not equal", DisplayName = "ModioModID != ModioModID"))
	static bool NotEqualTo(const FModioModID& A, const FModioModID& B);

	/**
	 * @brief Create entitlement parameters
	 * @param ExtendedParameters A map to store extended parameters required by some portals
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioEntitlementParams MakeEntitlementParams(const TMap<FString, FString>& ExtendedParameters);

	/**
	 * @brief Create Metrics Session parameters
	 * @param Ids The list of mods to store within the Metrics Session parameters
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioMetricsSessionParams MakeMetricsSessionParams(const TArray<FModioModID>& Ids);

	/**
	 * @brief Create a Guid from a string
	 * @param Guid A string to wrap within a Guid struct
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioGuid MakeGuid(const FString Guid);

	/**
	 * @brief Changes the session identifier for the provided Metrics Session parameters
	 * @param Params The template Metrics Session parameters
	 * @param Id The intended Guid to store in the Metrics Session parameters
	 */
	UFUNCTION(BlueprintPure, category = "mod.io|Utilities", meta = (NativeMakeFunc))
	static FModioMetricsSessionParams SetSessionId(const FModioMetricsSessionParams& Params, const FModioGuid& Id);
};