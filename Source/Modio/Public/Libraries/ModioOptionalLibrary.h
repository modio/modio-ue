/*
 *  Copyright (C) 2024-2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioGameInfo.h"
#include "Types/ModioImageWrapper.h"
#include "Types/ModioModDependencyList.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioModInfoList.h"
#include "Types/ModioModProgressInfo.h"
#include "Types/ModioModTagOptions.h"
#include "Types/ModioTerms.h"
#include "Types/ModioUnsigned64.h"
#include "Types/ModioModCollection.h"

#include "ModioOptionalLibrary.generated.h"

/**
 * @docpublic
 * @brief Utility library for handling optional types used in the mod.io API
 */
UCLASS()
class MODIO_API UModioOptionalLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Check if the mod info list has a valid value
	 *
	 * @param OptionalModInfoList - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModInfoList)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModInfoList(const struct FModioOptionalModInfoList& OptionalModInfoList);

	/**
	 * @docpublic
	 * @brief Get the mod info list from the optional if it's set
	 *
	 * @param OptionalModInfoList -
	 * @param ModInfoList - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  DisplayName = "GetValue (ModioOptionalModInfoList)")
	static bool GetValue_ModioOptionalModInfoList(const struct FModioOptionalModInfoList& OptionalModInfoList,
												  struct FModioModInfoList& ModInfoList);

	/**
	 * @docpublic
	 * @brief Check if the Game info list has a valid value
	 *
	 * @param OptionalGameInfoList - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalGameInfoList)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalGameInfoList(const struct FModioOptionalGameInfoList& OptionalGameInfoList);

	/**
	 * @docpublic
	 * @brief Get the game info list from the optional if it's set
	 *
	 * @param OptionalGameInfoList -
	 * @param GameInfoList - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  DisplayName = "GetValue (ModioOptionalGameInfoList)")
	static bool GetValue_ModioOptionalGameInfoList(const struct FModioOptionalGameInfoList& OptionalGameInfoList,
												   struct FModioGameInfoList& GameInfoList);

	/**
	 * @docpublic
	 * @brief Check if the mod info has a valid value
	 *
	 * @param OptionalModInfo - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModInfo)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModInfo(const struct FModioOptionalModInfo& OptionalModInfo);

	/**
	 * @docpublic
	 * @brief Get the mod info from the optional if it's set
	 *
	 * @param OptionalModInfo -
	 * @param ModInfo - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalModInfo)")
	static bool GetValue_ModioOptionalModInfo(const struct FModioOptionalModInfo& OptionalModInfo,
											  struct FModioModInfo& ModInfo);

	/**
	 * @docpublic
	 * @brief Check if the game info has a valid value
	 *
	 * @param OptionalGameInfo - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalGameInfo)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalGameInfo(const struct FModioOptionalGameInfo& OptionalGameInfo);

	/**
	 * @docpublic
	 * @brief Get the mod info from the optional if it's set
	 *
	 * @param OptionalGameInfo -
	 * @param GameInfo - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalGameInfo)")
	static bool GetValue_ModioOptionalGameInfo(const struct FModioOptionalGameInfo& OptionalGameInfo,
											   struct FModioGameInfo& GameInfo);

	/**
	 * @docpublic
	 * @brief Check if the user optional has a valid value
	 *
	 * @param OptionalUser - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalUser)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalUser(const struct FModioOptionalUser& OptionalUser);

	/**
	 * @docpublic
	 * @brief Get the user from the optional if it's set
	 *
	 * @param OptionalUser -
	 * @param User - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalUser)")
	static bool GetValue_ModioOptionalUser(const struct FModioOptionalUser& OptionalUser, struct FModioUser& User);

	/**
	 * @docpublic
	 * @brief Check if the optional mod tag option has a valid value
	 *
	 * @param OptionalModTagOptions - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModTagOptions)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModTagOptions(const struct FModioOptionalModTagOptions& OptionalModTagOptions);

	/**
	 * @docpublic
	 * @brief Get the mod tag option from the optional if it's set
	 *
	 * @param OptionalModTagOptions -
	 * @param ModTagOptions - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  DisplayName = "GetValue (ModioOptionalModTagOptions)")
	static bool GetValue_ModioOptionalModTagOptions(const struct FModioOptionalModTagOptions& OptionalModTagOptions,
													struct FModioModTagOptions& ModTagOptions);

	/**
	 * @docpublic
	 * @brief Check if the mod progression info has a valid value
	 *
	 * @param OptionalModProgressInfo - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModProgressInfo)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModProgressInfo(const struct FModioOptionalModProgressInfo& OptionalModProgressInfo);

	/**
	 * @docpublic
	 * @brief Get the mod progression info from the optional if it's set
	 *
	 * @param OptionalModProgressInfo -
	 * @param ModProgressInfo - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  DisplayName = "GetValue (ModioOptionalModProgressInfo)")
	static bool GetValue_ModioOptionalModProgressInfo(
		const struct FModioOptionalModProgressInfo& OptionalModProgressInfo,
		struct FModioModProgressInfo& ModProgressInfo);

	/**
	 * @docpublic
	 * @brief Check if the image has a valid value
	 *
	 * @param OptionalImage - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalImage)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalImage(const struct FModioOptionalImage& OptionalImage);

	/**
	 * @docpublic
	 * @brief Get the image from the optional if it's set
	 *
	 * @param OptionalImage -
	 * @param Image - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalImage)")
	static bool GetValue_ModioOptionalImage(const struct FModioOptionalImage& OptionalImage,
											struct FModioImageWrapper& Image);

	/**
	 * @docpublic
	 * @brief Check if the terms has a valid value
	 *
	 * @param OptionalTerms - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalTerms)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalTerms(const struct FModioOptionalTerms& OptionalTerms);

	/**
	 * @docpublic
	 * @brief Get the terms from the optional if it's set
	 *
	 * @param OptionalTerms -
	 * @param Terms - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalTerms)")
	static bool GetValue_ModioOptionalTerms(const struct FModioOptionalTerms& OptionalTerms, struct FModioTerms& Terms);

	/**
	 * @docpublic
	 * @brief Check if the optional mod dependency list has a valid value
	 *
	 * @param OptionalDependencyList - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModDependencyList)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModDependencyList(
		const struct FModioOptionalModDependencyList& OptionalDependencyList);

	/**
	 * @docpublic
	 * @brief Get the dependency list from the optional if it's set
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
	 * @docpublic
	 * @brief Check if the ModioModID has a valid value
	 *
	 * @param OptionalID - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioModID)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModID(const struct FModioOptionalModID& OptionalID);

	/**
	 * @docpublic
	 * @brief Get the ID from the optional if it's set
	 *
	 * @param OptionalID - if this returned false, then this will be defaulted
	 * @param ID - the underlying value
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalID)")
	static bool GetValue_ModioOptionalModID(const struct FModioOptionalModID& OptionalID, struct FModioModID& ID);

	/**
	 * @docpublic
	 * @brief Check if the FModioUnsigned64 has a valid value
	 *
	 * @param OptionalInt - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalUInt64)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalUInt64(const struct FModioOptionalUInt64& OptionalInt);

	/**
	 * @docpublic
	 * @brief Get the FModioUnsigned64 from the optional if it's set
	 *
	 * @param OptionalInt - if this returned false, then this will be defaulted
	 * @param UnderlyingInt - the underlying value
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalUInt64)")
	static bool GetValue_ModioOptionalUInt64(const struct FModioOptionalUInt64& OptionalInt,
											 FModioUnsigned64& UnderlyingInt);

	/**
	 * @docpublic
	 * @brief Check if the optional mod change map has a valid value
	 *
	 * @param OptionalModChangeMap - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
		meta = (DisplayName = "IsSet (ModioOptionalModChangeMap)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModChangeMap(const struct FModioOptionalModChangeMap& OptionalModChangeMap);

	/**
	 * @docpublic
	 * @brief Get the mod change map from the optional if it's set
	 *
	 * @param OptionalModChangeMap - The optional to get the value from
	 * @param ModChangeMap - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
		DisplayName = "GetValue (ModioOptionalModChangeMap)")
	static bool GetValue_ModioOptionalModChangeMap(const struct FModioOptionalModChangeMap& OptionalModChangeMap,
	                                               struct FModioModChangeMap& ModChangeMap);

	/**
	 * @docpublic
	 * @brief Check if the mod collection info has a valid value
	 *
	 * @param OptionalModCollectionInfo - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModCollectionInfo)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModCollectionInfo(const struct FModioOptionalModCollectionInfo& OptionalModCollectionInfo);

	/**
	 * @docpublic
	 * @brief Get the mod collection info from the optional if it's set
	 *
	 * @param OptionalModCollectionInfo -
	 * @param ModCollectionInfo - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional", DisplayName = "GetValue (ModioOptionalModCollectionInfo)")
	static bool GetValue_ModioOptionalModCollectionInfo(
		const struct FModioOptionalModCollectionInfo& OptionalModCollectionInfo,
		struct FModioModCollectionInfo& ModCollectionInfo);

	/**
	 * @docpublic
	 * @brief Check if the mod Collection info list has a valid value
	 *
	 * @param OptionalModCollectionInfoList - The optional to check
	 * @return true if it has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  meta = (DisplayName = "IsSet (ModioOptionalModCollectionInfoList)", CompactNodeTitle = "IsSet"))
	static bool IsSet_ModioOptionalModCollectionInfoList(
		const struct FModioOptionalModCollectionInfoList& OptionalModCollectionInfoList);

	/**
	 * @docpublic
	 * @brief Get the mod Collection info list from the optional if it's set
	 *
	 * @param OptionalModCollectionInfoList -
	 * @param ModCollectionInfoList - if this returned false, then this will be defaulted
	 * @return true if the optional has a value set
	 */
	UFUNCTION(BlueprintPure, Category = "mod.io|Utilities|Optional",
			  DisplayName = "GetValue (ModioOptionalModCollectionInfoList)")
	static bool GetValue_ModioOptionalModCollectionInfoList(
		const struct FModioOptionalModCollectionInfoList& OptionalModCollectionInfoList,
		struct FModioModCollectionInfoList& ModCollectionInfoList);
};
