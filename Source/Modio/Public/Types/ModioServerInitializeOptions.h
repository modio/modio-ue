/*
 *  Copyright (C) 2025-2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "ModioInitializeOptions.h"

#include "ModioServerInitializeOptions.generated.h"

/**
 * @docpublic
 * @brief Struct type to store the initialization options used by the mod.io SDK in the context of a dedicated server
 * @experimental
 */
USTRUCT(BlueprintType, meta = (HasNativeMake = "/Script/Modio.ModioCommonTypesLibrary:MakeServerInitializeOptions"))
struct MODIO_API FModioServerInitializeOptions
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief The base initialize options required in any Mod.io initialization context.
	 * @experimental
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io", meta = (ShowOnlyInnerProperties))
	FModioInitializeOptions BaseOptions = FModioInitializeOptions();

	/**
	 * @docpublic
	 * @brief The directory where mods should be stored on this Server
	 * @experimental
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io")
	FString ModsDirectory = "";

	/**
	 * @docpublic
	 * @brief The S2S token for this Server
	 * @experimental
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io")
	FString Token = "";

	/**
	 * @docpublic
	 * @brief What mods this Server will use.
	 * @experimental
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io")
	TArray<FModioModID> Mods = TArray<FModioModID>();
};