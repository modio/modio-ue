/*
 *  Copyright (C) 2021-2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Containers/UnrealString.h"

#include "ModioLogo.generated.h"

/**
 * Contains media URLs to the logo for the game
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioLogo
{
	GENERATED_BODY()

	/** @brief Logo filename including extension. */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Logo")
	FString Filename;
	/** @brief URL to the full - sized logo. */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Logo")
	FString Original;
	/** @brief URL to the small logo thumbnail. */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Logo")
	FString Thumb320x180;
	/** @brief URL to the medium logo thumbnail. */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Logo")
	FString Thumb640x360;
	/** @brief URL to the large logo thumbnail. */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Logo")
	FString Thumb1280x720;
};
