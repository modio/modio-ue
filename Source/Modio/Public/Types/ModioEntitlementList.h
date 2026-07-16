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

#include "Types/ModioList.h"
#include "Types/ModioEntitlement.h"
#include "Types/ModioPagedResult.h"

#include "ModioEntitlementList.generated.h"


namespace Modio
{
	class EntitlementList;
}

#if CPP
/**
 * @docpublic
 * @brief Struct to bring forward a native version of FModioEntitlementList 
 */
struct MODIO_API FModioEntitlementList : public FModioPagedResult, public FModioList<TArray, FModioEntitlement>
{
	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioEntitlementList() = default;

	/**
	 * @docpublic
	 * @brief Constructor that takes an array mod info and a index of paged results
	 */
	FModioEntitlementList(const FModioPagedResult& PagedResult, TArray<FModioEntitlement>&& EntitlementList);

	/**
	 * @docpublic
	 * @brief Constructor that takes a mod info list to store in this struct
	 */
	FModioEntitlementList(const class Modio::EntitlementList& EntitlementList);
};

#else
/** 
 * @docpublic
 * @brief Strong type struct to wrap multiple Entitlement indexed by a paged result 
 */
USTRUCT(NoExport, BlueprintType)
struct MODIO_API FModioEntitlementList
{
	/**
	 * @docpublic
	 * @brief A paged result property
	 */
	UPROPERTY()
	FModioPagedResult PagedResult;

	/**
	 * @docpublic
	 * @brief Array of mod tag info
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|Entitlements")
	TArray<FModioEntitlement> InternalList;
};

#endif

/**
 * @docpublic
 * @brief Struct to wrap EntitlementList into an optional parameter
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioOptionalEntitlementList
{
	GENERATED_BODY()
	
	/**
	 * @docpublic
	 * @brief Default constructor without parameters
	 */
	FModioOptionalEntitlementList() = default;
	
	/**
	 * @docpublic
	 * @brief Constructor with a mod info list parameter to initialize an instance
	 * @param EntitlementList Optional value of a FModioEntitlementList
	 */
	FModioOptionalEntitlementList(TOptional<FModioEntitlementList>&& EntitlementList);

	/**
	 * @docpublic
	 * @brief Stored optional ModioEntitlementList
	 */
	TOptional<FModioEntitlementList> Internal;
};