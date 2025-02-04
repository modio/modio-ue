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
#include "ModioCommonTypes.h"
#include "ModioUnsigned64.h"

#include "ModioTransactionRecord.generated.h"

namespace Modio
{
	struct TransactionRecord;
}

/**
 * @docpublic
 * @brief Contains information about a successful transaction for a mod
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioTransactionRecord
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioTransactionRecord() = default;

	/** 
	 * @docpublic
	 * @brief Constructor that uses a transaction record to initialize an instance
	 * @param Other The transaction record to use in the construction of this struct
	 */
	FModioTransactionRecord(const Modio::TransactionRecord& Other);

	/** 
	 * @docpublic
	 * @brief The mod the purchase was for
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Monetization|TransactionRecord")
	FModioModID AssociatedModID;

	/** 
	 * @docpublic
	 * @brief The price the mod was purchased for
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Monetization|TransactionRecord")
	FModioUnsigned64 Price;

	/** 
	 * @docpublic
	 * @brief The updated balance in the user's wallet after the transaction
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Monetization|TransactionRecord")
	FModioUnsigned64 UpdatedUserWalletBalance;
};

/**
 * @docpublic
 * @brief Struct to store an optional value for the transaction record
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalTransactionRecord
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief The Modio transaction record as an optional value
	 */
	TOptional<FModioTransactionRecord> Internal;
};
