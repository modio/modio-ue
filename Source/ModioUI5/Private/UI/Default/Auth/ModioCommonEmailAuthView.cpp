/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/Auth/ModioCommonEmailAuthView.h"

#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBox.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Settings/Params/ModioCommonAuthParams.h"
#include "Libraries/ModioSDKLibrary.h"

void UModioCommonEmailAuthView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonEmailAuthParamsSettings* Settings = GetDefault<UModioCommonEmailAuthParamsSettings>())
	{
		if (TitleTextBlock)
		{
			TitleTextBlock->SetText(Settings->TitleText);
		}

		if (DescriptionTextBlock)
		{
			DescriptionTextBlock->SetText(Settings->DescriptionText);
		}

		if (ValidationTextBlock)
		{
			ValidationTextBlock->SetText(Settings->ValidationText);
		}

		if (EmailTextBox)
		{
			EmailTextBox->SetHintText(Settings->EmailTooltipText);
		}

		if (BackButton)
		{
			BackButton->SetLabel(Settings->BackButtonText);
		}

		if (SubmitButton)
		{
			SubmitButton->SetLabel(Settings->SubmitButtonText);
		}

		if (CancelButton)
		{
			CancelButton->SetLabel(Settings->CancelButtonText);
		}
	}
}

void UModioCommonEmailAuthView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	const UModioCommonAuthParamsSettings* AuthSettings = GetDefault<UModioCommonAuthParamsSettings>();
	const UModioCommonEmailAuthParamsSettings* EmailAuthSettings = GetDefault<UModioCommonEmailAuthParamsSettings>();
	if (!AuthSettings || !EmailAuthSettings)
	{
		return;
	}

	if (BackButton)
	{
		ListenForInputAction(BackButton, AuthSettings->BackInputAction, EmailAuthSettings->BackButtonText, [this]() {
			if (OnBackClicked.IsBound()) OnBackClicked.Execute();
		});
	}

	if (SubmitButton)
	{
		ListenForInputAction(SubmitButton, AuthSettings->SubmitInputAction, EmailAuthSettings->SubmitButtonText, [this]() {
			if (EmailTextBox && OnSubmitClicked.IsBound()) OnSubmitClicked.Execute(EmailTextBox->GetText().ToString());
		});
	}

	if (CancelButton)
	{
		ListenForInputAction(CancelButton, AuthSettings->CancelInputAction, EmailAuthSettings->CancelButtonText, [this]() {
			if (OnCancelClicked.IsBound()) OnCancelClicked.Execute();
		});
	}

	if (EmailTextBox)
	{
		EmailTextBox->OnEditableTextChanged.AddDynamic(this, &UModioCommonEmailAuthView::OnEditableTextBoxTextChanged);
		SetValidationTextVisibility(ESlateVisibility::Hidden);
	}
}

UWidget* UModioCommonEmailAuthView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	return EmailTextBox;
}

void UModioCommonEmailAuthView::SetValidationTextVisibility_Implementation(ESlateVisibility Visbility)
{
	if (ValidationTextBlock)
	{
		ValidationTextBlock->SetVisibility(Visbility);
	}
}

bool UModioCommonEmailAuthView::IsEmailValid_Implementation(const FString& Email)
{
	if (!UModioSDKLibrary::IsValidEmailAddressFormat(Email))
	{
		SetValidationTextVisibility(ESlateVisibility::Visible);
		return false;
	}

	SetValidationTextVisibility(ESlateVisibility::Hidden);
	return true;
}

void UModioCommonEmailAuthView::OnEditableTextBoxTextChanged_Implementation(const FText& InText)
{
	SetValidationTextVisibility(ESlateVisibility::Hidden);
}