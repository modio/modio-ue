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

#include "ModioIcon.generated.h"

/**
 * @docpublic
 * @brief Contains media URLs to the icon for the game
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioIcon
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Icon filename including extension
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Icon")
	FString Filename;

	/**
	 * @docpublic
	 * @brief URL to the full-sized icon
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Icon")
	FString Original;

	/**
	 * @docpublic
	 * @brief URL to the small icon thumbnail
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Icon")
	FString Thumb64x64;

	/**
	 * @docpublic
	 * @brief URL to the medium icon thumbnail
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Icon")
	FString Thumb128x128;

	/**
	 * @docpublic
	 * @brief URL to the large icon thumbnail
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Icon")
	FString Thumb256x256;
};