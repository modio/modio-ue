/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Interfaces/IModioUIModEnableWidget.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "Loc/BeginModioLocNamespace.h"

void IModioUIModEnableWidget::ModEnabledStateChangedHandler(FModioModID ModID, bool bNewSubscriptionState)
{
	bRoutedModEnabledStateChanged = false;
	NativeOnModEnabledStateChanged(ModID, bNewSubscriptionState);
	checkf(bRoutedModEnabledStateChanged,
		   TEXT("Please call IModioUIModEnableWidget::NativeOnModEnabledStateChanged in your derived "
				"implementation in order to route notifications to blueprint."));
}

void IModioUIModEnableWidget::NativeOnModEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	bRoutedModEnabledStateChanged = true;
	Execute_OnModEnabledStateChanged(Cast<UObject>(this), ModID, bNewSubscriptionState);
}

bool IModioUIModEnableWidget::QueryModEnabled(FModioModID ModID)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		return Subsystem->QueryIsModEnabled(ModID);
	}
	else
	{
		return false;
	}
}

void IModioUIModEnableWidget::RequestModEnabledState(FModioModID ModID, bool bNewStateIsEnabled)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		bool bSuccess = Subsystem->RequestModEnabledState(ModID, bNewStateIsEnabled) && bNewStateIsEnabled;
		Subsystem->DisplayErrorNotificationManual(
			bSuccess ? LOCTEXT("ModEnabled", "Mod enabled") : LOCTEXT("ModDisabled", "Mod disabled"),
												  FText(), !bSuccess);
	}
}

#include "Loc/EndModioLocNamespace.h"