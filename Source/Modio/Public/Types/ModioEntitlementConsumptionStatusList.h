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
 * @docpublic
 * @brief Updated wallet balance from the sync entitlements call
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioEntitlementWalletBalance
{
	GENERATED_BODY();

	/**
	 * @docpublic
	 * @brief The updated balance of the wallet
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FModioUnsigned64 Balance;
};

/**
 * @docpublic
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
 * @docpublic
 * @brief Type of entitlement that was consumed
 */
UENUM(BlueprintType)
enum class EModioEntitlementType : uint8
{
	VirtualCurrency = 0
};

/**
 * @docpublic
 * @brief Optional updated wallet balance from the sync entitlements call
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalEntitlementWalletBalance
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioOptionalEntitlementWalletBalance() = default;

	/**
	 * @docpublic
	 * @brief Convenience constructor that has entitlement wallet balance
	 * @param EntitlementWalletBalance Entitlement wallet balance to store
	 */
	FModioOptionalEntitlementWalletBalance(TOptional<FModioEntitlementWalletBalance>&& EntitlementWalletBalance)
		: Internal(EntitlementWalletBalance) {}

	/**
	 * @docpublic
	 * @brief Stored optional ModioEntitlementWalletBalance
	 */
	TOptional<FModioEntitlementWalletBalance> Internal;
};

/**
 * @docpublic
 * @brief Further details about a Virtual Currency entitlement that was consumed
 */
USTRUCT(BlueprintType)
struct FModioEntitlementConsumptionVirtualCurrencyDetails
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Amount of tokens that were issued for this specific entitlement consumption
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	int32 TokensAllocated = 0;
};

/**
 * @docpublic
 * @brief The result of an entitlement's consumption
 */
USTRUCT(BlueprintType)
struct FEntitlementConsumptionStatus
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief ID of the transaction to redeem this entitlement
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FString TransactionId;

	/**
	 * @docpublic
	 * @brief State of the transaction
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	EModioEntitlementConsumptionState TransactionState = EModioEntitlementConsumptionState::Failed;

	/**
	 * @docpublic
	 * @brief ID of the SKU that we attempted to consume
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FString SkuId = "";

	/**
	 * @docpublic
	 * @brief Whether this entitlement was consumed or not
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	bool EntitlementConsumed = false;

	/**
	 * @docpublic
	 * @brief Type of Entitlement that was consumed
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	EModioEntitlementType EntitlementType = EModioEntitlementType::VirtualCurrency;

	/**
	 * @docpublic
	 * @brief Details about virtual currency entitlement consumption
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FModioEntitlementConsumptionVirtualCurrencyDetails VirtualCurrencyDetails;
};

/**
 * @docpublic
 * @brief Class representing a list of entitlement consumption statuses that may be a page from a larger set of results
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioEntitlementConsumptionStatusList
{
	GENERATED_BODY();

	/**
	 * @docpublic
	 * @brief Stored property for a paged result, which provides context on the entitlement consumption status list
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FModioPagedResult PagedResult;

	/**
	 * @docpublic
	 * @brief Stored property for the dependency list
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	TArray<FEntitlementConsumptionStatus> InternalList;

	/**
	 * @docpublic
	 * @brief Updated wallet balance from syncing entitlements
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	FModioOptionalEntitlementWalletBalance WalletBalance;
};

/**
 * @docpublic
 * @brief Optional class representing a list of entitlement consumption statuses that may be a page from a larger set of results
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalEntitlementConsumptionStatusList
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioOptionalEntitlementConsumptionStatusList() = default;

	/**
	 * @docpublic
	 * @brief Convenience constructor that has entitlement consumption status list
	 * @param ModDependencies Entitlement consumption status list to store
	 */
	FModioOptionalEntitlementConsumptionStatusList(TOptional<FModioEntitlementConsumptionStatusList>&& ModDependencies)
		: Internal(ModDependencies) {}

	/**
	 * @docpublic
	 * @brief Stored optional ModioEntitlementConsumptionStatusList
	 */
	TOptional<FModioEntitlementConsumptionStatusList> Internal;
};