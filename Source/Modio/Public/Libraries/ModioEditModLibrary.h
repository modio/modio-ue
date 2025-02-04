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
#include "Types/ModioEditModParams.h"
#include "Types/ModioModInfo.h"

#include "ModioEditModLibrary.generated.h"

/**
 * @docpublic
 * @brief Utility functions for editing mod parameters
 */
UCLASS()
class MODIO_API UModioEditModLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * @brief Set the name of the mod
	 * @param In The edit mod parameters to modify
	 * @param Name The new name of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetName(UPARAM(ref) FModioEditModParams& In, FString Name);

	/**
	 * @brief Set the summary of the mod
	 * @param In The edit mod parameters to modify
	 * @param Summary The new summary of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetSummary(UPARAM(ref) FModioEditModParams& In, FString Summary);

	/**
	 * @brief Set the name path of the mod
	 * @param In The edit mod parameters to modify
	 * @param NamePath The new name path of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetNamePath(UPARAM(ref) FModioEditModParams& In, FString NamePath);

	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params",
			  meta = (DeprecatedProperty, DeprecationMessage = "Deprecated as of 2023.6 release. Please use `SetVisibility` with `EModioObjectVisibilityFlags Visibility`  instead."))
	static void SetVisibility_DEPRECATED(UPARAM(ref) FModioEditModParams& In, bool Visibility);

	/**
	 * @brief Set the visibility of the mod
	 * @param In The edit mod parameters to modify
	 * @param Visibility The new visibility of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetVisibility(UPARAM(ref) FModioEditModParams& In, EModioObjectVisibilityFlags Visibility);

	/**
	 * @brief Set the description of the mod
	 * @param In The edit mod parameters to modify
	 * @param Description The new description of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetDescription(UPARAM(ref) FModioEditModParams& In, FString Description);

	/**
	 * @brief Set the homepage URL of the mod
	 * @param In The edit mod parameters to modify
	 * @param HomepageURL The new homepage URL of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetHomepageURL(UPARAM(ref) FModioEditModParams& In, FString HomepageURL);

	/**
	 * @brief Set the maturity flags of the mod
	 * @param In The edit mod parameters to modify
	 * @param MaturityFlags The new maturity flags of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetMaturityFlags(UPARAM(ref) FModioEditModParams& In, EModioMaturityFlags MaturityFlags);
	
	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetCommunityOptionsFlags(UPARAM(ref) FModioEditModParams& In, EModioModCommunityOptionsFlags CommunityOptionsFlags);

	/**
	 * @brief Set the metadata blob of the mod
	 * @param In The edit mod parameters to modify
	 * @param MetadataBlob The new metadata blob of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetMetadataBlob(UPARAM(ref) FModioEditModParams& In, FString MetadataBlob);

	/**
	 * @brief Set the logo path of the mod
	 * @param In The edit mod parameters to modify
	 * @param LogoPath The new logo path of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetLogoPath(UPARAM(ref) FModioEditModParams& In, FString LogoPath);

	/**
	 * @brief Set the tags of the mod
	 * @param In The edit mod parameters to modify
	 * @param Tags The new tags of the mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Edit Mod Params")
	static void SetTags(UPARAM(ref) FModioEditModParams& In, UPARAM(ref) TArray<FString>& Tags);
};
