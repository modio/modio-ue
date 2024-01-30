/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/Auth/ModioCommonEmailAuthLoadingView.h"

#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Settings/ModioCommonUISettings.h"
#include "UI/Settings/Params/ModioCommonAuthParams.h"

void UModioCommonEmailAuthLoadingView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (TitleTextBlock)
		{
			TitleTextBlock->SetText(UISettings->EmailAuthLoadingParams.TitleText);
		}

		if (DescriptionTextBlock)
		{
			DescriptionTextBlock->SetText(UISettings->EmailAuthLoadingParams.DescriptionText);
		}

		if (CancelButton)
		{
			CancelButton->SetLabel(UISettings->EmailAuthLoadingParams.CancelButtonText);
		}
	}
}

void UModioCommonEmailAuthLoadingView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (CancelButton)
		{
			ListenForInputAction(CancelButton, UISettings->AuthParams.CancelInputAction, UISettings->EmailAuthLoadingParams.CancelButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				OnCancelClicked.Broadcast();
			}));
		}
	}
}

UWidget* UModioCommonEmailAuthLoadingView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	return CancelButton;
}
