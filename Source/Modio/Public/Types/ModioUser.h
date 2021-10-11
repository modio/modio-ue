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

#include "Types/ModioCommonTypes.h"

#include "ModioUser.generated.h"


namespace Modio
{
	struct User;
}

/** @brief Object representing a mod.io user profile **/
USTRUCT(BlueprintType)
struct MODIO_API FModioUser
{
	GENERATED_BODY()

	FModioUser() = default;
	FModioUser(const Modio::User& User);

	/** @brief Unique id for the user */
	UPROPERTY(BlueprintReadOnly,Category="User")
	FModioUserID UserId;

	/** @brief Username of the user */
	UPROPERTY(BlueprintReadOnly,Category="User")
	FString Username;

	/** @brief Unix timestamp the user was last online */
	UPROPERTY(BlueprintReadOnly,Category="User")
	FDateTime DateOnline;

	/** @brief URL of the user's mod.io profile */
	UPROPERTY(BlueprintReadOnly,Category="User")
	FString ProfileUrl;
};

USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalUser
{
	GENERATED_BODY()

	TOptional<FModioUser> Internal;
};