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

#include "Misc/DateTime.h"
#include "Types/ModioCommonTypes.h"
#include "Misc/DateTime.h"

#include "ModioUser.generated.h"

namespace Modio
{
	struct User;
}

/**
 * Object representing a mod.io user profile information
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioUser
{
	GENERATED_BODY()

	FModioUser() = default;
	FModioUser(const Modio::User& User);

	/**
	 * User's unique ID
	 **/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User")
	FModioUserID UserId;

	/**
	 * Name chosen by the user
	 **/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User")
	FString Username;

	/**
	 * Unix timestamp the last time the  user was online
	 **/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User")
	FDateTime DateOnline;

	/**
	 * URL of the user's mod.io profile
	 **/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User")
	FString ProfileUrl;

	/**
	 * Display name of this User for the given Portal, if their account is linked
	 **/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User")
	FString DisplayNamePortal;
};

/**
 * Struct wrapper for an optional ModioUser
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalUser
{
	GENERATED_BODY()

	/**
	 * Stored optional ModioUser
	 **/
	TOptional<FModioUser> Internal;
};