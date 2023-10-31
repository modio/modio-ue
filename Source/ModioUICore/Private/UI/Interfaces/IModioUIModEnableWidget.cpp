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
#include "Core/ModioModInfoUI.h"
#include "Loc/BeginModioLocNamespace.h"

void IModioUIModEnableWidget::ModEnabledStateChangedHandler_Implementation(FModioModID ModID, bool bNewSubscriptionState)
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
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModInfoAsync(ModID, FOnGetModInfoDelegateFast::CreateWeakLambda(Cast<UObject>(this), [this, ModID, bNewStateIsEnabled](FModioErrorCode ErrorCode, TOptional<FModioModInfo> ModInfo)
		{
			if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
			{
				
				const bool bSuccess = UISubsystem->RequestModEnabledState(ModID, bNewStateIsEnabled) && !ErrorCode;
				// We currently don't want to show a notification for the end user
				//
				//const FText DescriptionText = bSuccess
				//	                              ? (bNewStateIsEnabled
				//		                                 ? LOCTEXT("ModEnabled", "Mod enabled")
				//		                                 : LOCTEXT("ModDisabled", "Mod disabled"))
				//	                              : (ErrorCode
				//		                                 ? FText::Format(
				//			                                 LOCTEXT("ErrorCode", "{0}: {1}"),
				//			                                 FText::FromString(ErrorCode.GetErrorMessage()),
				//			                                 FText::AsNumber(ErrorCode))
				//		                                 : LOCTEXT("RequestModEnabledStateFailed", "Mod enable request failed"));
				//
				//UISubsystem->DisplayNotificationManual(FText::FromString(ModInfo.Get(FModioModInfo()).ProfileName), DescriptionText, !bSuccess);
			}
		}));
	}
}
	

#include "Loc/EndModioLocNamespace.h"