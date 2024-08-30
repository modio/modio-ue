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
#include "Types/ModioCommonTypes.h"
#include "Types/ModioUnsigned64.h" 
#include "Types/ModioPagedResult.h"
#include "Containers/UnrealString.h"
#include "Misc/Optional.h"

#include "ModioEntitlementConsumptionStatusList.generated.h"

/**
 * @brief Updated wallet balance from the sync entitlements call
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioEntitlementWalletBalance
{
	GENERATED_BODY();

	/**
	 * @brief The updated balance of the wallet
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FModioUnsigned64 Balance;
};

/**
 * @brief State of an entitlement consumption transaction
 */
UENUM(BlueprintType)
enum class EModioEntitlementConsumptionState : uint8
{
	Failed = 0,
	Pending = 1, 
	Fulfilled = 2,
	ConsumeLimitExceeded = 3
};

/**
 * @brief Type of entitlement that was consumed
 */
UENUM(BlueprintType)
enum class EModioEntitlementType : uint8
{
	VirtualCurrency = 0
};

/**
 * @brief Optional updated wallet balance from the sync entitlements call
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalEntitlementWalletBalance
{
	GENERATED_BODY()

	/**
	 * @brief Default constructor without parameters
	 */
	FModioOptionalEntitlementWalletBalance() = default;

	/**
	 * @brief Convenience constructor that has entitlement wallet balance
	 * @param EntitlementWalletBalance Entitlement wallet balance to store
	 */
	FModioOptionalEntitlementWalletBalance(TOptional<FModioEntitlementWalletBalance>&& EntitlementWalletBalance)
		: Internal(EntitlementWalletBalance) {}

	/**
	 * @brief Stored optional ModioEntitlementWalletBalance
	 */
	TOptional<FModioEntitlementWalletBalance> Internal;
};

/**
 * @brief Further details about a Virtual Currency entitlement that was consumed
 */
USTRUCT(BlueprintType)
struct FModioEntitlementConsumptionVirtualCurrencyDetails
{
	GENERATED_BODY()

	/**
	 * @brief Amount of tokens that were issued for this specific entitlement consumption
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	int32 TokensAllocated = 0;
};

/**
 * @brief The result of an entitlement's consumption
 */
USTRUCT(BlueprintType)
struct FEntitlementConsumptionStatus
{
	GENERATED_BODY()

	/**
	 * @brief ID of the transaction to redeem this entitlement
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FString TransactionId;

	/**
	 * @brief State of the transaction
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	EModioEntitlementConsumptionState TransactionState = EModioEntitlementConsumptionState::Failed;

	/**
	 * @brief ID of the SKU that we attempted to consume
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FString SkuId = "";

	/**
	 * @brief Whether this entitlement was consumed or not
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	bool EntitlementConsumed;

	/**
	 * @brief Type of Entitlement that was consumed
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	EModioEntitlementType EntitlementType = EModioEntitlementType::VirtualCurrency;

	/**
	 * @brief Details about virtual currency entitlement consumption
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FModioEntitlementConsumptionVirtualCurrencyDetails VirtualCurrencyDetails;
};

/**
 * @brief Class representing a list of entitlement consumption statuses that may be a page from a larger set of results
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioEntitlementConsumptionStatusList
{
	GENERATED_BODY();

	/**
	 * @brief Stored property for a paged result, which provides context on the entitlement consumption status list
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FModioPagedResult PagedResult;

	/**
	 * @brief Stored property for the dependency list
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	TArray<FEntitlementConsumptionStatus> InternalList;

	/**
	 * @brief Updated wallet balance from syncing entitlements
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FModioOptionalEntitlementWalletBalance WalletBalance;
};

/**
 * @brief Optional class representing a list of entitlement consumption statuses that may be a page from a larger set of results
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalEntitlementConsumptionStatusList
{
	GENERATED_BODY()

	FModioOptionalEntitlementConsumptionStatusList() = default;

	FModioOptionalEntitlementConsumptionStatusList(TOptional<FModioEntitlementConsumptionStatusList>&& ModDependencies)
		: Internal(ModDependencies) {}

	/**
	 * @brief Stored optional ModioEntitlementConsumptionStatusList
	 */
	TOptional<FModioEntitlementConsumptionStatusList> Internal;
};