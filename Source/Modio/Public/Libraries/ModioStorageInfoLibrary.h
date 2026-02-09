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
#include "Types/ModioStorageInfo.h"
#include "Types/ModioUnsigned64.h"

#include "ModioStorageInfoLibrary.generated.h"

/**
 *
 */
UCLASS()
class MODIO_API UModioStorageInfoLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief Returns the recorded storage space for a given storage location and usage type
	 * @param Location Where the storage has been written to
	 * @param Usage The type of storage usage recorded
	 * @return Total storage space in bytes
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Storage Info")
	static FModioUnsigned64 GetSpace(const FModioStorageInfo& Info, EModioStorageLocation Location,
									 EModioStorageUsage Usage)
	{
		return Info.GetSpace(Location, Usage);
	}

	/**
	 * @brief Returns the recorded storage space for a given storage location and usage type
	 * @param StorageKey Where the storage has been written to and what type of usage to query
	 * @return Total storage space in bytes
	 **/
	UFUNCTION(BlueprintPure, Category = "mod.io|Storage Info")
	static FModioUnsigned64 GetSpaceByKey(const FModioStorageInfo& Info, FStorageInfoKey StorageKey)
	{
		return Info.GetSpace(StorageKey);
	}
};