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

#include "ModioTheme.generated.h"

/**
 * Theme color values for the game
 **/
USTRUCT(BlueprintType)
struct MODIO_API FModioTheme
{
	GENERATED_BODY()

	/** @brief The primary hex color code */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Primary;
	/** @brief The dark hex color code */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Dark;
	/** @brief The light hex color code */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Light;
	/** @brief The success hex color code */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Success;
	/** @brief The warning hex color code */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Warning;
	/** @brief The danger hex color code */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Danger;
};