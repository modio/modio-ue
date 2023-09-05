/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/Auth/ModioCommonTermsOfUseView.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Settings/Params/ModioCommonAuthParams.h"

void UModioCommonTermsOfUseView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	const UModioCommonAuthParamsSettings* AuthSettings = GetDefault<UModioCommonAuthParamsSettings>();
	const UModioCommonTermsOfUseParamsSettings* TermsOfUseSettings = GetDefault<UModioCommonTermsOfUseParamsSettings>();

	if (!AuthSettings || !TermsOfUseSettings)
	{
		return;
	}

	if (TitleTextBlock)
	{
		TitleTextBlock->SetText(TermsOfUseSettings->TitleText);
	}

	if (ContentTextBlock)
	{
		ContentTextBlock->SetText(FText::FromString(Terms.TermsText));
	}
	if (CancelButton)
	{
		CancelButton->SetLabel(FText::FromString(Terms.DisagreeButtonText));
	}
	if (SubmitButton)
	{
		SubmitButton->SetLabel(FText::FromString(Terms.AgreeButtonText));
	}

	ClearListeningInputActions();
	if (SubmitButton)
	{
		ListenForInputAction(SubmitButton, AuthSettings->SubmitInputAction, FText::FromString(Terms.AgreeButtonText), [this]() {
			if (OnSubmitClicked.IsBound()) OnSubmitClicked.Execute();
		});
	}

	if (CancelButton)
	{
		ListenForInputAction(CancelButton, AuthSettings->CancelInputAction, FText::FromString(Terms.DisagreeButtonText), [this]() {
			if (OnCancelClicked.IsBound()) OnCancelClicked.Execute();
		});
	}
	BindInputActions();
}

UWidget* UModioCommonTermsOfUseView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	return SubmitButton;
}
