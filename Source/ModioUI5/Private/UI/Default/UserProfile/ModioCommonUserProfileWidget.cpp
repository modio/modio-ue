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
#include "UI/Settings/Params/ModioCommonUserProfileWidgetParams.h"

UModioCommonUserProfileWidget::UModioCommonUserProfileWidget()
{
	bAutoFocusOnActivation = false;
	bAutoBindInputAction = false;
}

void UModioCommonUserProfileWidget::NativeOnInitialized()
{
	if (const UModioCommonUserProfileWidgetParamsSettings* Settings = GetDefault<UModioCommonUserProfileWidgetParamsSettings>())
	{
		ListenForInputAction(ProfileButton, Settings->ProfileInputAction, Settings->ProfileButtonLabel, [this]() {
			if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
			{
				if (UModioCommonModBrowser* ModBrowser = Cast<UModioCommonModBrowser>(Subsystem->ModBrowserInstance))
				{
					ModBrowser->ShowQuickAccessView();
				}
			}
		});
	}
	Super::NativeOnInitialized();
}

void UModioCommonUserProfileWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	UnbindInputActions();
	BindInputActions();
}

void UModioCommonUserProfileWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	UnbindInputActions();
}
