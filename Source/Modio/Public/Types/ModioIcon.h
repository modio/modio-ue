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

#include "ModioIcon.generated.h"

/**
 * Contains media URLs to the icon for the game
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioIcon
{
	GENERATED_BODY()

	/** @brief Icon filename including extension. */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Icon")
	FString Filename;
	/** @brief URL to the full-sized icon. */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Icon")
	FString Original;
	/** @brief URL to the small icon thumbnail. */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Icon")
	FString Thumb64x64;
	/** @brief URL to the medium icon thumbnail. */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Icon")
	FString Thumb128x128;
	/** @brief URL to the large icon thumbnail. */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Icon")
	FString Thumb256x256;
};