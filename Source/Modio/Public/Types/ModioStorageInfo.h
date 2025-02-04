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
 * Where the storage has been written to
 **/
UENUM(BlueprintType)
enum class EModioStorageLocation : uint8
{
	Local,

	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EModioStorageLocation, EModioStorageLocation::Count);

/**
 * The type of storage usage
 **/
UENUM(BlueprintType)
enum class EModioStorageUsage : uint8
{
	Consumed,
	Available,

	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EModioStorageUsage, EModioStorageUsage::Count);

/**
 * A key used to query storage information by location and usage
 **/
USTRUCT(BlueprintType)
struct MODIO_API FStorageInfoKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io|Storage Info")
	EModioStorageLocation Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io|Storage Info")
	EModioStorageUsage Usage;

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
 * Strong type struct containing storage usage information including availability and consumption
 **/
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