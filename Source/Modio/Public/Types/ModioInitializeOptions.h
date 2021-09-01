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

// clang-format off
#include "ModioInitializeOptions.generated.h"
// clang-format on

USTRUCT(BlueprintType, meta = (HasNativeMake = "Modio.ModioCommonTypesLibrary.MakeInitializeOptions"))
struct FModioInitializeOptions
{
	GENERATED_BODY()

	/** @brief The Mod.io-provided ID for the game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io")
	FModioGameID GameId = FModioGameID::InvalidGameID();

	/** @brief The Mod.io-provided API key for your application or game */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io")
	FModioApiKey ApiKey = FModioApiKey::InvalidAPIKey();

	/** @brief The mod.io environment you want to run the SDK on */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io")
	EModioEnvironment GameEnvironment = EModioEnvironment::Test;

	/** @brief The portal your title is running through */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io")
	EModioPortal PortalInUse = EModioPortal::None;
};
