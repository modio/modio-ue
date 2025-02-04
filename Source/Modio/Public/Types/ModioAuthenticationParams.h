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

#include "Containers/UnrealString.h"

#include "ModioAuthenticationParams.generated.h"

/**
 * @docpublic
 * @brief Simple struct to encapsulate data passed to external authentication systems
 */
UENUM(BlueprintType)
enum class EModioAuthenticationProvider : uint8
{
	XboxLive,
	Steam,
	GoG,
	Itch,
	Switch,
	Discord,
	PSN, 
	Epic,
	Oculus,
	OpenID,
	GoogleIDToken,
	GoogleServerSideToken
};

/**
 * @docpublic
 * @brief Struct to encapsulate data passed to external authentication systems
 * 
 * This struct is used to store authentication parameters required by various external 
 * authentication providers. These parameters are used to authenticate users with 
 * external systems (e.g., Xbox, Steam, Google, etc.).
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioAuthenticationParams
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Stored property of the authentication token
	 * This token is required for authentication with the external provider.
	 */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "mod.io|Authentication")
	FString AuthToken;

	/**
	 * @docpublic
	 * @brief Stored property of the user email
	 * This is the email address of the user, which is used for authentication 
	 * purposes in certain external systems.
	 */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "mod.io|Authentication")
	FString UserEmail;

	/**
	 * @docpublic
	 * @brief Boolean that signals if the user has accepted the terms of service
	 * This flag is used to indicate whether the user has agreed to the terms of service 
	 * required by the external authentication provider.
	 */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "mod.io|Authentication")
	bool bUserHasAcceptedTerms = false;

	/**
	 * @docpublic
	 * @brief Stored property of extended parameters required by some providers
	 * This map holds additional key-value pairs for authentication that might be 
	 * required by specific authentication providers. These could include things like 
	 * additional credentials or metadata.
	 */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "mod.io|Authentication")
	TMap<FString, FString> ExtendedParameters;
};
