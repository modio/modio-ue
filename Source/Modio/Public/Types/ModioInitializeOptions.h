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

#include "ModioInitializeOptions.generated.h"

USTRUCT(BlueprintType, meta = (HasNativeMake = "Modio.ModioCommonTypesLibrary.MakeInitializeOptions"))
struct MODIO_API FModioInitializeOptions
{
	GENERATED_BODY()

	/** @brief The Mod.io-provided ID for the game */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io", meta=(ShowOnlyInnerProperties))
	FModioGameID GameId = FModioGameID::InvalidGameID();

	/** @brief The Mod.io-provided API key for your application or game */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io", meta=(ShowOnlyInnerProperties))
	FModioApiKey ApiKey = FModioApiKey::InvalidAPIKey();

	/** @brief The mod.io environment you want to run the SDK on */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io")
	EModioEnvironment GameEnvironment = EModioEnvironment::Test;

	/** @brief The portal your title is running through */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io")
	EModioPortal PortalInUse = EModioPortal::None;

	/// @brief String representing the local session. We recommend setting this to a unique per-user string.
	/// On Windows, defaults to the string SID of the current logged-in Windows account. 
	/// On non-desktop platforms, this needs to be set by gameplay code before passing `FModioInitializeOptions` to `InitializeAsync`
	TOptional<FString> LocalSessionIdentifier;
};
