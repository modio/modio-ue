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

#include "ModioHeaderImage.generated.h"

/**
 * @docpublic
 * @brief Contains media URLs to the preview header image for the game
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioHeaderImage
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Header image filename including extension
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|HeaderImage")
	FString Filename;

	/**
	 * @docpublic
	 * @brief URL to the full-sized header image
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|HeaderImage")
	FString Original;
};