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

#include "ModioHeaderImage.generated.h"

/**
 * Contains media URLs to the preview header image for the game
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioHeaderImage
{
	GENERATED_BODY()

	/** @brief  Header image filename including extension */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|HeaderImage")
	FString Filename;
	/** @brief  URL to the full-sized header image */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|HeaderImage")
	FString Original;
};