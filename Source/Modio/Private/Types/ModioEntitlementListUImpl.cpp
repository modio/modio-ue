/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Types/ModioEntitlementList.h"

#include "Internal/Convert/Entitlements.h"

#include "Internal/Convert/List.h"
#include "ModioSDK.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioEntitlementList)

template const TArray<FModioEntitlement>& FModioList<TArray, FModioEntitlement>::GetRawList() const;

FModioEntitlementList::FModioEntitlementList(const Modio::EntitlementList& EntitlementList)
	: FModioPagedResult(EntitlementList)
{
    TArray<FModioEntitlement> Result;

    Result.Reserve(size_t(EntitlementList.Size()));
    for (const Modio::Entitlement& It : EntitlementList)
    {
        Result.Emplace(ToUnreal(It));
    }

    InternalList = Result;
}

FModioEntitlementList::FModioEntitlementList(const FModioPagedResult& PagedResult, TArray<FModioEntitlement>&& EntitlementList)
	: FModioPagedResult(PagedResult),
	  FModioList(MoveTemp(EntitlementList))
{}

FModioOptionalEntitlementList::FModioOptionalEntitlementList(TOptional<FModioEntitlementList>&& EntitlementList)
	: Internal(MoveTemp(EntitlementList))
{}
