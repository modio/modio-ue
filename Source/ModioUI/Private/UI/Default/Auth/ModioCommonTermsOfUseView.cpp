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
#include "UI/Settings/ModioCommonUISettings.h"
#include "UI/Settings/Params/ModioCommonAuthParams.h"

void UModioCommonTermsOfUseView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (TitleTextBlock)
		{
			TitleTextBlock->SetText(UISettings->TermsOfUseParams.TitleText);
		}

		if (ContentTextBlock)
		{
			ContentTextBlock->SetText(Terms.TermsText.IsEmpty() ? UISettings->TermsOfUseParams.ContentText : FText::FromString(Terms.TermsText));
		}
		if (TermsButton)
		{
			TermsButton->SetLabel(Terms.TermsLink.Text.IsEmpty() ? UISettings->TermsOfUseParams.TermsButtonText : FText::FromString(Terms.TermsLink.Text));
		}
		if (PrivacyButton)
		{
			PrivacyButton->SetLabel(Terms.PrivacyLink.Text.IsEmpty() ? UISettings->TermsOfUseParams.PrivacyButtonText : FText::FromString(Terms.PrivacyLink.Text));
		}

		if (CancelButton)
		{
			CancelButton->SetLabel(Terms.DisagreeButtonText.IsEmpty() ? UISettings->TermsOfUseParams.CancelButtonText : FText::FromString(Terms.DisagreeButtonText));
		}
		if (SubmitButton)
		{
			SubmitButton->SetLabel(Terms.AgreeButtonText.IsEmpty() ? UISettings->TermsOfUseParams.SubmitButtonText : FText::FromString(Terms.AgreeButtonText));
		}

		ClearListeningInputActions();
		if (SubmitButton)
		{
			ListenForInputAction(SubmitButton, UISettings->AuthParams.SubmitInputAction, FText::FromString(Terms.AgreeButtonText),
								 FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
								 	OnSubmitClicked.Broadcast();
								 }));
		}

		if (CancelButton)
		{
			ListenForInputAction(CancelButton, UISettings->AuthParams.CancelInputAction, FText::FromString(Terms.DisagreeButtonText),
								 FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
								 	OnCancelClicked.Broadcast();
								 }));
		}
		if (TermsButton)
		{
			ListenForInputAction(TermsButton, UISettings->AuthParams.TermsInputAction, FText::FromString(Terms.TermsLink.Text),
								 FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
									 OnLinkClicked.Broadcast(Terms.TermsLink.URL);
								 }));
		}
		if (PrivacyButton)
		{
			ListenForInputAction(PrivacyButton, UISettings->AuthParams.PrivacyInputAction, FText::FromString(Terms.PrivacyLink.Text),
								 FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
									 OnLinkClicked.Broadcast(Terms.PrivacyLink.URL);
								 }));
		}
		BindInputActions();
	}
}

UWidget* UModioCommonTermsOfUseView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	return SubmitButton;
}
