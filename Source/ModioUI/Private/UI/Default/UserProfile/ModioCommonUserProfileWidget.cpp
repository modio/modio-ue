/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/UserProfile/ModioCommonUserProfileWidget.h"

#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"
#include "UI/Foundation/Base/ModBrowser/ModioCommonModBrowserBase.h"
#include "UI/Settings/ModioCommonUISettings.h"
#include "UI/Settings/Params/ModioCommonUserProfileWidgetParams.h"

UModioCommonUserProfileWidget::UModioCommonUserProfileWidget()
{
	bAutoFocusOnActivation = false;
}

void UModioCommonUserProfileWidget::NativeOnInitialized()
{
	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (ProfileButton)
		{
			ListenForInputAction(ProfileButton, UISettings->UserProfileParams.ProfileInputAction, UISettings->UserProfileParams.ProfileButtonLabel, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
				{
					if (UModioCommonModBrowser* ModBrowser = Cast<UModioCommonModBrowser>(Subsystem->GetModBrowserInstance()))
					{
						ModBrowser->ShowQuickAccessView();
					}
				}
			}));
		}
	}
	Super::NativeOnInitialized();
}
