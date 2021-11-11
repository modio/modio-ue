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
#include "Types/ModioCreateModParams.h"
#include "Templates/EnableIf.h"

template<typename DestinationType, typename SourceType>
FORCEINLINE typename TEnableIf<TNot<TIsSame<SourceType,DestinationType>>::Value, typename Modio::Optional<DestinationType>>::Type ToModioOptional(const TOptional<SourceType>& In)
{
	if (In.IsSet())
	{
		return ToModio((SourceType) In.GetValue());
	}
	else
	{
		return {};
	}
}

template<typename DestinationType, typename SourceType>
FORCEINLINE typename TEnableIf<TIsSame<SourceType,DestinationType>::Value, typename Modio::Optional<DestinationType>>::Type ToModioOptional(const TOptional<SourceType>& In)
{
    if (In.IsSet())
    {
        return In.GetValue();
    }
    else
    {
        return {};
    }
} 