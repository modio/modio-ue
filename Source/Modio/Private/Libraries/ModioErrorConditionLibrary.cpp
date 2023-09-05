/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Libraries/ModioErrorConditionLibrary.h"
#include "ModioSDK.h"

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
