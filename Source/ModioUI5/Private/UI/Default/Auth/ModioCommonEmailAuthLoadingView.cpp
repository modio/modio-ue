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
#include "UI/Settings/Params/ModioCommonAuthParams.h"

void UModioCommonEmailAuthLoadingView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonEmailAuthLoadingParamsSettings* Settings = GetDefault<UModioCommonEmailAuthLoadingParamsSettings>())
	{
		if (TitleTextBlock)
		{
			TitleTextBlock->SetText(Settings->TitleText);
		}

		if (DescriptionTextBlock)
		{
			DescriptionTextBlock->SetText(Settings->DescriptionText);
		}

		if (CancelButton)
		{
			CancelButton->SetLabel(Settings->CancelButtonText);
		}
	}
}

void UModioCommonEmailAuthLoadingView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const UModioCommonAuthParamsSettings* AuthSettings = GetDefault<UModioCommonAuthParamsSettings>();
	const UModioCommonEmailAuthLoadingParamsSettings* AuthLoadingSettings = GetDefault<UModioCommonEmailAuthLoadingParamsSettings>();
	
	if (!AuthSettings || !AuthLoadingSettings)
	{
		return;
	}

	if (CancelButton)
	{
		ListenForInputAction(CancelButton, AuthSettings->CancelInputAction, AuthLoadingSettings->CancelButtonText, [this]() {
			if (OnCancelClicked.IsBound()) OnCancelClicked.Execute();
		});
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
