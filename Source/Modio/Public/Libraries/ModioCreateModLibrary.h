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

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioCreateModFileParams.h"
#include "Types/ModioCreateModParams.h"
#include "Types/ModioEditModParams.h"
#include "Types/ModioModInfo.h"

#include "ModioCreateModLibrary.generated.h"

/**
 * @docpublic
 * @brief Utility functions for creating mod and mod file parameters
 */
UCLASS()
class MODIO_API UModioCreateModLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params",
			  meta = (DeprecatedProperty,
					  DeprecationMessage = "Deprecated as of 2023.6 release. Please use the `SetInitialVisibility` instead."))
	static void SetInitialVisibility_DEPRECATED(UPARAM(ref) FModioCreateModParams& In, bool InitialVisibility);

	/**
	 * @docpublic
	 * @brief Sets the initial visibility of the mod
	 * @param In The mod creation parameters to modify
	 * @param InitialVisibility The initial visibility of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	static void SetInitialVisibility(UPARAM(ref) FModioCreateModParams& In, EModioObjectVisibilityFlags InitialVisibility);

	/**
	 * @docpublic
	 * @brief Sets the description of the mod
	 * @param In The mod creation parameters to modify
	 * @param Description The description of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	static void SetDescription(UPARAM(ref) FModioCreateModParams& In, FString Description);

	/**
	 * @docpublic
	 * @brief Sets the homepage URL of the mod
	 * @param In The mod creation parameters to modify
	 * @param HomepageURL The homepage URL of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	static void SetHomepageURL(UPARAM(ref) FModioCreateModParams& In, FString HomepageURL);

	/**
	 * @docpublic
	 * @brief Sets the metadata blob of the mod
	 * @param In The mod creation parameters to modify
	 * @param MetadataBlob The metadata blob of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	static void SetMaturityFlags(UPARAM(ref) FModioCreateModParams& In, EModioMaturityFlags MaturityFlags);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	static void SetCommunityOptionsFlags(UPARAM(ref) FModioCreateModParams& In, EModioModCommunityOptionsFlags CommunityOptionsFlags);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	static void SetMetadataBlob(UPARAM(ref) FModioCreateModParams& In, FString MetadataBlob);

	/**
	 * @docpublic
	 * @brief Sets tags for the mod
	 * @param In The mod creation parameters to modify
	 * @param Tags The tags for the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod Params")
	static void SetTags(UPARAM(ref) FModioCreateModParams& In, UPARAM(ref) TArray<FString>& Tags);

	/**
	 * @docpublic
	 * @brief Sets the version string of the mod file
	 * @param In The mod file creation parameters to modify
	 * @param Version The version string of the mod file
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod File Params")
	static void SetVersionString(UPARAM(ref) FModioCreateModFileParams& In, FString Version);

	/**
	 * @docpublic
	 * @brief Sets the changelog string of the mod file
	 * @param In The mod file creation parameters to modify
	 * @param Changelog The changelog string of the mod file
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod File Params")
	static void SetChangelogString(UPARAM(ref) FModioCreateModFileParams& In, FString Changelog);

	/**
	 * @docpublic
	 * @brief Sets the MarkAsActiveRelease flag of the mod file
	 * @param In The mod file creation parameters to modify
	 * @param bMarkAsActiveRelease The MarkAsActiveRelease flag of the mod file
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod File Params")
	static void SetMarkAsActiveRelease(UPARAM(ref) FModioCreateModFileParams& In, bool bMarkAsActiveRelease);

	/**
	 * @docpublic
	 * @brief Sets the mod file metadata blob of the mod file
	 * @param In The mod file creation parameters to modify
	 * @param MetadataBlob The mod file metadata blob of the mod file
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod File Params")
	static void SetModFileMetadataBlob(UPARAM(ref) FModioCreateModFileParams& In, FString MetadataBlob);

	/**
	 * @docpublic
	 * @brief Sets the mod file platforms of the mod file
	 * @param In The mod file creation parameters to modify
	 * @param Platforms The mod file platforms of the mod file
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Create Mod File Params")
	static void SetModfilePlatforms(UPARAM(ref) FModioCreateModFileParams& In,
									UPARAM(ref) TArray<EModioModfilePlatform>& Platforms);
};
