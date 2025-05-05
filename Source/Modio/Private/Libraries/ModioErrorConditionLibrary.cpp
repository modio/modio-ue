/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioErrorConditionLibrary.h"
#include "ModioSDK.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioErrorConditionLibrary)

bool UModioErrorConditionLibrary::ErrorCodeMatches(FModioErrorCode ErrorCode, EModioErrorCondition Condition)
{
	if (Condition == EModioErrorCondition::NoError)
	{
	//If we wanted to match against NoError, any error at all is a failed match
		if ((bool) ErrorCode.GetRawErrorCode())
		{
			return false;
		}
		return true;
	}
	return Modio::ErrorCodeMatches(ErrorCode.GetRawErrorCode(), static_cast<Modio::ErrorConditionTypes>(Condition));
}
