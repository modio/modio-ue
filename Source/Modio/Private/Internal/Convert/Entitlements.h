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

#include "Internal/ModioConvert.h"

FORCEINLINE FEntitlementConsumptionStatus ToUnreal(const Modio::EntitlementConsumptionStatus& In)
{
	FEntitlementConsumptionStatus Out;
	Out.TransactionId = ToUnreal(In.TransactionId);
	Out.TransactionState = ToUnreal(In.TransactionState);
	Out.SkuId = ToUnreal(In.SkuId);
	Out.EntitlementConsumed = In.EntitlementConsumed;
	Out.EntitlementType = ToUnreal(In.EntitlementType);
	Out.VirtualCurrencyDetails = ToUnreal(In.VirtualCurrencyDetails);
	return Out;
}

FORCEINLINE FModioEntitlementConsumptionVirtualCurrencyDetails ToUnreal(const Modio::EntitlementConsumptionVirtualCurrencyDetails& In)
{
	FModioEntitlementConsumptionVirtualCurrencyDetails Out;
	Out.TokensAllocated = In.TokensAllocated;
	return Out;
}

FORCEINLINE FModioEntitlementWalletBalance ToUnreal(const Modio::EntitlementWalletBalance& In)
{
	FModioEntitlementWalletBalance Out;
	Out.Balance = FModioUnsigned64(In.Balance);
	return Out;
}

FORCEINLINE EModioEntitlementConsumptionState ToUnreal(const Modio::EntitlementConsumptionState& In)
{
	switch (In)
	{
		case Modio::EntitlementConsumptionState::Failed:
			return EModioEntitlementConsumptionState::Failed;
		case Modio::EntitlementConsumptionState::Pending:
			return EModioEntitlementConsumptionState::Pending;
		case Modio::EntitlementConsumptionState::Fulfilled:
			return EModioEntitlementConsumptionState::Fulfilled;
		case Modio::EntitlementConsumptionState::ConsumeLimitExceeded:
			return EModioEntitlementConsumptionState::ConsumeLimitExceeded;
		default:
			return EModioEntitlementConsumptionState::Failed;
	}
}

FORCEINLINE EModioEntitlementType ToUnreal(const Modio::EntitlementType& In)
{
	switch (In)
	{
		case Modio::EntitlementType::VirtualCurrency:
			return EModioEntitlementType::VirtualCurrency;
		default:
			return EModioEntitlementType::VirtualCurrency;
	}
}