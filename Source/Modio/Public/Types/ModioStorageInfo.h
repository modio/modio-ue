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

#include "Types/ModioCommonTypes.h"
#include "Types/ModioUnsigned64.h"

#include "ModioStorageInfo.generated.h"

namespace Modio
{
	struct StorageInfo;
}

/**
 * @brief Identifies the backing location used to store data.
 **/
UENUM(BlueprintType)
enum class EModioStorageLocation : uint8
{
	Local,
	Cache,

	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EModioStorageLocation, EModioStorageLocation::Count);

/**
 * @brief Indicates how a storage amount should be interpreted when reporting disk usage.
 *
 * Use `Consumed` for space already used by mod.io data, and `Available` for free space that can be used
 * for downloads/installs.
 */
UENUM(BlueprintType)
enum class EModioStorageUsage : uint8
{
	Consumed,
	Available,

	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EModioStorageUsage, EModioStorageUsage::Count);

/**
 * @brief Composite key for looking up storage metrics by location and usage type.
 *
 * Combines an `EModioStorageLocation` (where the data lives) with an `EModioStorageUsage`
 * (whether reporting consumed vs. available space). Implements equality and hashing so it can be used
 * as a key in hashed containers (e.g., `TMap`).
 */
USTRUCT(BlueprintType)
struct MODIO_API FStorageInfoKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io|Storage Info")
	EModioStorageLocation Location = EModioStorageLocation::Local;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io|Storage Info")
	EModioStorageUsage Usage = EModioStorageUsage::Consumed;

	bool operator==(const FStorageInfoKey& Other) const
	{
		return Location == Other.Location && Usage == Other.Usage;
	}

	friend uint32 GetTypeHash(const FStorageInfoKey& Pair)
	{
		return HashCombine(GetTypeHash(Pair.Location), GetTypeHash(Pair.Usage));
	}
};

/**
 * @brief Strongly-typed container for storage space metrics (consumed/available) across storage locations.
 *
 * Stores storage values (in bytes) keyed by `EModioStorageLocation` and `EModioStorageUsage`, and provides
 * convenience accessors via `GetSpace(...)` to query the recorded amount for a given key.
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioStorageInfo
{
	GENERATED_BODY()

	/**
	 * Default constructor without parameters
	 **/
	FModioStorageInfo() = default;

	/**
	 * Returns the recorded storage space for a given storage location and usage type
	 * @param Location Where the storage has been written to
	 * @param Usage The type of storage usage recorded
	 * @return Total storage space in bytes
	 **/
	FModioUnsigned64 GetSpace(EModioStorageLocation Location, EModioStorageUsage Usage) const
	{
		return GetSpace(FStorageInfoKey {Location, Usage});
	}

	/**
	 * Returns the recorded storage space for a given storage location and usage type
	 * @param StorageKey Where the storage has been written to and what type of usage to query
	 * @return Total storage space in bytes
	 **/
	FModioUnsigned64 GetSpace(FStorageInfoKey StorageKey) const
	{
		if (StorageData.Contains(StorageKey))
		{
			return StorageData[StorageKey];
		}
		return FModioUnsigned64(0);
	}

private:
	friend FModioStorageInfo ToUnreal(const Modio::StorageInfo& In);

	friend void SetSpace(FModioStorageInfo& Info, EModioStorageLocation Location, EModioStorageUsage Usage,
						 FModioUnsigned64 Space);

	/// @docnone
	TMap<FStorageInfoKey, FModioUnsigned64> StorageData;
};