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

#include "ModioTheme.generated.h"

/**
 * @docpublic
 * @brief Theme color values for the game
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioTheme
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief The primary hex color code
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Primary;

	/** 
	 * @docpublic
	 * @brief The dark hex color code
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Dark;

	/** 
	 * @docpublic
	 * @brief The light hex color code
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Light;

	/** 
	 * @docpublic
	 * @brief The success hex color code
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Success;

	/** 
	 * @docpublic
	 * @brief The warning hex color code
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Warning;

	/** 
	 * @docpublic
	 * @brief The danger hex color code
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Theme")
	FString Danger;
};