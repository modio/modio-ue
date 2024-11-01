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
#include "ModioSDK.h"
#include "Types/ModioTransactionRecord.h"

FORCEINLINE FModioTransactionRecord ToUnreal(const Modio::TransactionRecord& In)
{
	FModioTransactionRecord Out;
	Out.AssociatedModID = ToUnreal(In.AssociatedModID);
	Out.Price.Underlying = In.Price;
	Out.UpdatedUserWalletBalance.Underlying = In.UpdatedUserWalletBalance;
	return Out;
}