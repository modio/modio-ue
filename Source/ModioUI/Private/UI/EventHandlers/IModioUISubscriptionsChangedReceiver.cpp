/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"

void IModioUISubscriptionsChangedReceiver::SubscriptionsChangedHandler(FModioModID ModID, bool bNewSubscriptionState)
{
	bRoutedSubscriptionsChanged = false;
	TRACE_CPUPROFILER_EVENT_SCOPE_TEXT(
		*(Cast<UObject>(this)->GetFName().ToString().Append("_SubscriptionsChangedHandler")));
	NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	checkf(bRoutedSubscriptionsChanged,
		   TEXT("Please call IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged in your derived "
				"implementation in order to route notifications to blueprint."));
}

void IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	bRoutedSubscriptionsChanged = true;
	Execute_OnSubscriptionsChanged(Cast<UObject>(this), ModID, bNewSubscriptionState);
}