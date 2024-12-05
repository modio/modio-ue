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
	 * upload method accepts a block of memory `TArray<uint8>` rather than a file path. The upload's progress can be
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
	 * accepts a block of memory `TArray<uint8>` rather than a file path. The
	 * upload's progress can be tracked in the same way as downloads; when completed, a Mod Management Event will be
	 * triggered with the result code for the upload.
	 * @param Target The ModioSubsystem object
	 * @param Mod The ID of the mod you are submitting a file for
	 * @param Params Information about the mod file being created, including the memory that wiull be uploaded as a mod
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitNewModFileForModFromMemory", Category = "mod.io|Mods|Submission")
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
	UFUNCTION(BlueprintCallable, DisplayName = "LoadModFileToMemory", Category = "mod.io|Mods")
	static MODIOEX_API bool K2_LoadModFileToMemory(UModioSubsystem* Target, FModioModID ModId, TArray<uint8>& ModData);

	/**
	 * @brief Submit a new mod, with its logo data coming from an in-memory buffer rather than a file.
	 * @param Handle Mod creation handle
	 * @param Params Parameters to use when creating the mod
	 * @param PngData In-memory buffer, representative of a PNG file to be used for upload
	 * @param Callback Callback once operation has completed
	 */
	static MODIOEX_API void SubmitNewModFromMemoryAsync(FModioModCreationHandle Handle, FModioCreateModParams Params, TArray<uint8> PngData, FOnSubmitNewModDelegateFast Callback);

	/**
	 * @brief Submit a new mod, with its logo data coming from an in-memory buffer rather than a file.
	 * @param Handle Mod creation handle
	 * @param Params Parameters to use when creating the mod
	 * @param PngData In-memory buffer, representative of a PNG file to be used for upload
	 * @param Callback Callback once operation has completed
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitNewModFromMemoryAsync", Category = "mod.io|Mods")
	static MODIOEX_API void K2_SubmitNewModFromMemoryAsync(FModioModCreationHandle Handle, FModioCreateModParams Params, TArray<uint8> PngData, FOnSubmitNewModDelegate Callback);

	/**
	 * @brief Edits the parameters of a mod, by updating any fields set in the Params object to match the passed-in,
	 * values. Fields left empty on the Params object will not be updated. This method also accepts a Png binary file
	 * for uploading a new logo.
	 * @param Mod The ID of the mod you wish to edit
	 * @param Params Descriptor containing the fields that should be altered.
	 * @param Callback The callback invoked when the changes have been submitted, containing an optional updated ModInfo
	 * object if the edits were performed successfully
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory InvalidArgsError|Some fields in Params did not pass validation
	 * @error UserDataError::InvalidUser|No authenticated user
	 */
	MODIOEX_API void SubmitModChangesFromMemoryAsync(FModioModID Mod, FModioEditModParams Params, TArray<uint8> PngData,
													 FOnGetModInfoDelegateFast Callback);

	/**
	 * @brief Edits the parameters of a mod, by updating any fields set in the Params object to match the passed-in
	 * values. Fields left empty on the Params object will not be updated. This method also accepts a Png binary file
	 * for uploading a new logo.
	 * @param Mod The ID of the mod you wish to edit
	 * @param Params Descriptor containing the fields that should be altered.
	 *
	 * @param Callback The callback invoked when the changes have been submitted, containing an optional updated ModInfo
	 * object if the edits were performed successfully
	 * @requires initialized-sdk
	 * @requires authenticated-user
	 * @requires no-rate-limiting
	 * @errorcategory NetworkError|Couldn't connect to mod.io servers
	 * @error GenericError::SDKNotInitialized|SDK not initialized
	 * @errorcategory InvalidArgsError|Some fields in Params did not pass validation
	 * @error UserDataError::InvalidUser|No authenticated user
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "SubmitModChangesAsync", Category = "mod.io|Mods|Editing")
	MODIOEX_API void K2_SubmitModChangesFromMemoryAsync(FModioModID Mod, FModioEditModParams Params, TArray<uint8> PngData, FOnGetModInfoDelegate Callback);
};