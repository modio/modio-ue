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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioModCollectionEntry.h"

#include "ModioModCollectionLibrary.generated.h"


/**
 * @docpublic
 * @brief Utility functions for accessing and managing mod collection data
 */
UCLASS()
class MODIO_API UModioModCollectionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @docpublic
	 * @brief Get the current state of a mod within the collection
	 * @param Entry The mod collection entry to retrieve the state from
	 * @return An EModioModState enum representing the current state of the mod
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Mods")
	static EModioModState GetModState(const FModioModCollectionEntry& Entry);

	/**
	 * @docpublic
	 * @brief Retrieve the Mod ID from the mod collection entry
	 * @param Entry The mod collection entry to retrieve the ID from
	 * @return A FModioModID representing the ID of the mod
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Mods")
	static FModioModID GetID(const FModioModCollectionEntry& Entry);

	/**
	 * @docpublic
	 * @brief Get the profile information of a mod
	 * @param Entry The mod collection entry to retrieve the profile information from
	 * @return An FModioModInfo structure containing the mod's profile data
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Mods")
	static const FModioModInfo& GetModProfile(const FModioModCollectionEntry& Entry);

	/**
	 * @docpublic
	 * @brief Get the installation path for a mod on disk
	 * @param Entry The mod collection entry to retrieve the installation path from
	 * @return A FString representing the path to the mod's installation folder. 
	 *         If the mod is not yet installed, this path may not exist. Use `GetModState` to check the mod's status before accessing files.
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Mods")
	static const FString GetPath(const FModioModCollectionEntry& Entry);
};
