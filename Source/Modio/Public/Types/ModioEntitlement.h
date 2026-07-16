/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Types/ModioCommonTypes.h"

#include "ModioEntitlement.generated.h"

namespace Modio
{
	struct Entitlement;
}

/**
 * @docpublic
 * @brief Information related to a SKU entitlement
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioEntitlement
{
	GENERATED_BODY()

	/** 
	 * @docpublic
	 * @brief Id of the Sku the entitlement is for
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Entitlement")
	FString SkuId;
	
	/** 
	 * @docpublic
	 * @brief The type of the entitlement
	 */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Entitlement")
	EModioEntitlementType EntitlementType;

	friend struct FModioEntitlement ToUnreal(const struct Modio::Entitlement& In);
};

/**
 * @docpublic
 * @brief Strong type struct to store an optional Entitlement parameter
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalEntitlement
{
	GENERATED_BODY()

	/**
	 * @docpublic
	 * @brief Stored property to an optional Entitlement
	 */
	TOptional<FModioEntitlement> Internal;
};
