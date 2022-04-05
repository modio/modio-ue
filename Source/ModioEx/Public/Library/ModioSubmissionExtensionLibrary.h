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

#include "ModioSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioCreateModFileMemoryParams.h"

#include "ModioSubmissionExtensionLibrary.generated.h"

UCLASS()
class UModioSubmissionExtensionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief Queues the upload of a new mod file release for the specified mod, using the submitted parameters. This
	 * upload method accepts a a block of memory TArray<uint8> rather than a file path. The upload's progress can be
	 * tracked in the same way as downloads; when completed, a Mod Management Event will be triggered with the result
	 * code for the upload.
	 * @param Target The ModioSubsystem object
	 * @param Mod The ID of the mod you are submitting a file for
	 * @param Params Information about the mod file being created, including the memory that wiull be uploaded as a mod
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 */
	static MODIOEX_API void SubmitNewModFileForModFromMemory(UModioSubsystem* Target, FModioModID Mod, FModioCreateModFileMemoryParams Params);

	/**
	 * @brief Queues the upload of a new mod file release for the specified mod, using the submitted parameters. This upload method
	 * accepts a a block of memory TArray<uint8> rather than a file path. The
	 * upload's progress can be tracked in the same way as downloads; when completed, a Mod Management Event will be
	 * triggered with the result code for the upload.
	 * @param Target The ModioSubsystem object
	 * @param Mod The ID of the mod you are submitting a file for
	 * @param Params Information about the mod file being created, including the memory that wiull be uploaded as a mod
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitNewModFileForModFromMemory", Category = "mods.io|Mods|Submission")
	static MODIOEX_API void K2_SubmitNewModFileForModFromMemory(UModioSubsystem *Target, FModioModID Mod, FModioCreateModFileMemoryParams Params);

	/**
	 * @brief Loads an installed mod file into memory.
	 * @param Target The ModioSubsystem object
	 * @param Mod The ID of the mod you are submitting a file for
	 * @param ModData A byte array of the mod that has been loaded
	 * @returns True if the mod file is successfully loaded; false if it cannot be loaded
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 */
	static MODIOEX_API bool LoadModFileToMemory(UModioSubsystem* Target, FModioModID ModId, TArray<uint8>& ModData);

	/**
	 * @brief Loads an installed mod file into memory. 
	 * @param Target The ModioSubsystem object
	 * @param Mod The ID of the mod you are submitting a file for
	 * @param ModData A byte array of the mod that has been loaded
	 * @returns True if the mod file is successfully loaded; false if it cannot be loaded
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "LoadModFileToMemory", Category = "mods.io|Mods")
	static MODIOEX_API bool K2_LoadModFileToMemory(UModioSubsystem* Target, FModioModID ModId, TArray<uint8>& ModData);

	
};