/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#pragma once
#include "Internal/ModioConvert.h"
#include "ModioSDK.h"
#include "Types/ModioTransactionRecord.h"

FORCEINLINE FModioTransactionRecord ToUnreal(const Modio::TransactionRecord& In)
{
	FModioTransactionRecord Out;
	Out.AssociatedMod = ToUnreal(In.AssociatedMod);
	Out.Price.Underlying = In.Price;
	Out.UpdatedUserWalletBalance.Underlying = In.UpdatedUserWalletBalance;
	return Out;
}