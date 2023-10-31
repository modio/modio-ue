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
		ContentTextBlock->SetText(Terms.TermsText.IsEmpty() ? TermsOfUseSettings->ContentText : FText::FromString(Terms.TermsText));
	}
	if (TermsButton)
	{
		TermsButton->SetLabel(Terms.TermsLink.Text.IsEmpty() ? TermsOfUseSettings->TermsButtonText : FText::FromString(Terms.TermsLink.Text));
	}
	if (PrivacyButton)
	{
		PrivacyButton->SetLabel(Terms.PrivacyLink.Text.IsEmpty() ? TermsOfUseSettings->PrivacyButtonText : FText::FromString(Terms.PrivacyLink.Text));
	}

	if (CancelButton)
	{
		CancelButton->SetLabel(Terms.DisagreeButtonText.IsEmpty() ? TermsOfUseSettings->CancelButtonText : FText::FromString(Terms.DisagreeButtonText));
	}
	if (SubmitButton)
	{
		SubmitButton->SetLabel(Terms.AgreeButtonText.IsEmpty() ? TermsOfUseSettings->SubmitButtonText : FText::FromString(Terms.AgreeButtonText));
	}

	ClearListeningInputActions();
	if (SubmitButton)
	{
		ListenForInputAction(SubmitButton, AuthSettings->SubmitInputAction, FText::FromString(Terms.AgreeButtonText),
							 [this]() {
								 if (OnSubmitClicked.IsBound())
									 OnSubmitClicked.Execute();
							 });
	}

	if (CancelButton)
	{
		ListenForInputAction(CancelButton, AuthSettings->CancelInputAction, FText::FromString(Terms.DisagreeButtonText),
							 [this]() {
								 if (OnCancelClicked.IsBound())
									 OnCancelClicked.Execute();
							 });
	}
	if (TermsButton)
	{
		ListenForInputAction(TermsButton, AuthSettings->TermsInputAction, FText::FromString(Terms.TermsLink.Text),
							 [this]() {
								 if (OnLinkClicked.IsBound())
									 OnLinkClicked.Execute(Terms.TermsLink.URL);
							 });
	}
	if (PrivacyButton)
	{
		ListenForInputAction(PrivacyButton, AuthSettings->PrivacyInputAction, FText::FromString(Terms.PrivacyLink.Text),
							 [this]() {
								 if (OnLinkClicked.IsBound())
									 OnLinkClicked.Execute(Terms.PrivacyLink.URL);
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
