/*
 *  Copyright (C) 2024-2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Misc/DateTime.h"
#include "Types/ModioCommonTypes.h"
#include "Misc/DateTime.h"

#include "ModioUser.generated.h"

namespace Modio
{
	struct User;
}

/**
 * @docpublic
 * @brief Object representing a mod.io user profile information
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioUser
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief Default constructor
	 */
	FModioUser() = default;

	/** 
	 * @docpublic
	 * @brief Constructor that initializes the FModioUser from a Modio::User
	 * @param User The Modio::User instance to copy data from
	 */
	FModioUser(const Modio::User& User);

	/** 
	 * @docpublic
	 * @brief User's unique ID
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User")
	FModioUserID UserId;

	/** 
	 * @docpublic
	 * @brief Name chosen by the user
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User")
	FString Username;

	/** 
	 * @docpublic
	 * @brief Unix timestamp of the last time the user was online
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User")
	FDateTime DateOnline;

	/** 
	 * @docpublic
	 * @brief URL of the user's mod.io profile
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User")
	FString ProfileUrl;

	/** 
	 * @docpublic
	 * @brief Display name of this User for the given Portal, if their account is linked
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User")
	FString DisplayNamePortal;

	bool operator==(const FModioUser& Other) const
	{
		return UserId == Other.UserId;
	}

	MODIO_API friend uint32 GetTypeHash(const FModioUser& User)
	{
		return FCrc::MemCrc32(&User.UserId, sizeof(FModioUserID));
	}
};

/**
 * @docpublic
 * @brief Struct wrapper for an optional ModioUser
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalUser
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief Stored optional ModioUser
	 */
	TOptional<FModioUser> Internal;
};
