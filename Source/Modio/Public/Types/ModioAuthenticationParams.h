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

#include "Containers/UnrealString.h"

#include "ModioAuthenticationParams.generated.h"

/** @brief Simple struct to encapsulate data passed to external authentication systems */
UENUM(BlueprintType)
enum class EModioAuthenticationProvider : uint8
{
	XboxLive,
	Steam,
	GoG,
	Itch,
	Switch,
	Discord
};

/** @brief Simple struct to encapsulate data passed to external authentication systems */
USTRUCT(BlueprintType)
struct MODIO_API FModioAuthenticationParams
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "mod.io|Authentication")
	FString AuthToken;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "mod.io|Authentication")
	FString UserEmail;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "mod.io|Authentication")
	bool bUserHasAcceptedTerms = false;
};
