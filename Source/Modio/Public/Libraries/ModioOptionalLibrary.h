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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioModInfoList.h"
#include "Types/ModioModProgressInfo.h"
#include "Types/ModioModTagOptions.h"
#include "Types/ModioTerms.h"
#include "Types/ModioImageWrapper.h"
#include "Types/ModioModDependencyList.h"

#include "ModioOptionalLibrary.generated.h"


UCLASS()
class MODIO_API UModioOptionalLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * Check if the mod info list has a valid value
	 *
	 * @param OptionalModInfoList - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModInfoList)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModInfoList(const struct FModioOptionalModInfoList& OptionalModInfoList);

	/**
	 * Get the mod info list from the optional if it's set
	 *
	 * @param OptionalModInfoList -
	 * @param ModInfoList - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalModInfoList)")
	static bool GetValue_ModioOptionalModInfoList(const struct FModioOptionalModInfoList& OptionalModInfoList,
												  struct FModioModInfoList& ModInfoList);

	/**
	 * Check if the mod info has a valid value
	 *
	 * @param OptionalModInfo - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModInfo)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModInfo(const struct FModioOptionalModInfo& OptionalModInfo);

	/**
	 * Get the mod info from the optional if it's set
	 *
	 * @param OptionalModInfo -
	 * @param ModInfo - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalModInfo)")
	static bool GetValue_ModioOptionalModInfo(const struct FModioOptionalModInfo& OptionalModInfo,
												  struct FModioModInfo& ModInfo);

	
	/**
	 * Check if the user optional has a valid value
	 *
	 * @param OptionalUser - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalUser)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalUser(const struct FModioOptionalUser& OptionalUser);

	/**
	 * Get the user from the optional if it's set
	 *
	 * @param OptionalUser -
	 * @param User - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalUser)")
	static bool GetValue_ModioOptionalUser(const struct FModioOptionalUser& OptionalUser, struct FModioUser& User);

	/**
	 * Check if the optional mod tag option has a valid value
	 *
	 * @param OptionalModTagOptions - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModTagOptions)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModTagOptions(const struct FModioOptionalModTagOptions& OptionalModTagOptions);

	/**
	 * Get the mod tag option from the optional if it's set
	 *
	 * @param OptionalModTagOptions -
	 * @param ModTagOptions - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalModTagOptions)")
	static bool GetValue_ModioOptionalModTagOptions(const struct FModioOptionalModTagOptions& OptionalModTagOptions,
													struct FModioModTagOptions& ModTagOptions);

	/**
	 * Check if the mod progression info has a valid value
	 *
	 * @param OptionalModProgressInfo - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModProgressInfo)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModProgressInfo(const struct FModioOptionalModProgressInfo& OptionalModProgressInfo);

	/**
	 * Get the mod progression info from the optional if it's set
	 *
	 * @param OptionalModProgressInfo -
	 * @param ModProgressInfo - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalModProgressInfo)")
	static bool GetValue_ModioOptionalModProgressInfo(
		const struct FModioOptionalModProgressInfo& OptionalModProgressInfo,
		struct FModioModProgressInfo& ModProgressInfo);

	/**
	 * Check if the image has a valid value
	 *
	 * @param OptionalImage - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalImage)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalImage(const struct FModioOptionalImage& OptionalImage);

	/**
	 * Get the image from the optional if it's set
	 *
	 * @param OptionalImage -
	 * @param Image - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalImage)")
	static bool GetValue_ModioOptionalImage(
		const struct FModioOptionalImage& OptionalImage,
		struct FModioImageWrapper& Image);

	/**
	 * Check if the terms has a valid value
	 *
	 * @param OptionalTerms - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalTerms)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalTerms(const struct FModioOptionalTerms& OptionalTerms);

	/**
	 * Get the terms from the optional if it's set
	 *
	 * @param OptionalTerms -
	 * @param Terms - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalTerms)")
	static bool GetValue_ModioOptionalTerms(
		const struct FModioOptionalTerms& OptionalTerms,
		struct FModioTerms& Terms);

	/**
	 * Check if the optional mod dependency list has a valid value
	 *
	 * @param OptionalDependencyList - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModDependencyList)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModDependencyList(
		const struct FModioOptionalModDependencyList& OptionalDependencyList);

	/**
	 * Get the dependency list from the optional if it's set
	 *
	 * @param OptionalDependencyList -
	 * @param DependencyList - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  DisplayName = "GetValue (ModioOptionalModDependencyList)")
	static bool GetValue_ModioOptionalModDependencyList(
		const struct FModioOptionalModDependencyList& OptionalDependencyList,
		struct FModioModDependencyList& DependencyList);


	/**
	 * Check if the ModioModID has a valid value
	 *
	 * @param OptionalID - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioModID)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModID(const struct FModioOptionalModID& OptionalID);

	/**
	 * Get the ID from the optional if it's set
	 *
	 * @param OptionalID - if this returned false, then this will be defaulted
	 * @param ID - the underlying value
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalID)")
	static bool GetValue_ModioOptionalModID(const struct FModioOptionalModID& OptionalID, struct FModioModID& ID);
};
