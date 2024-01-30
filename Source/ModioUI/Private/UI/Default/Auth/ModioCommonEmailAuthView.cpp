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
#include "UI/Settings/ModioCommonUISettings.h"

void UModioCommonEmailAuthView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (TitleTextBlock)
		{
			TitleTextBlock->SetText(UISettings->EmailAuthParams.TitleText);
		}

		if (DescriptionTextBlock)
		{
			DescriptionTextBlock->SetText(UISettings->EmailAuthParams.DescriptionText);
		}

		if (ValidationTextBlock)
		{
			ValidationTextBlock->SetText(UISettings->EmailAuthParams.ValidationText);
		}

		if (EmailTextBox)
		{
			EmailTextBox->SetHintText(UISettings->EmailAuthParams.EmailTooltipText);
		}

		if (BackButton)
		{
			BackButton->SetLabel(UISettings->EmailAuthParams.BackButtonText);
		}

		if (SubmitButton)
		{
			SubmitButton->SetLabel(UISettings->EmailAuthParams.SubmitButtonText);
		}

		if (CancelButton)
		{
			CancelButton->SetLabel(UISettings->EmailAuthParams.CancelButtonText);
		}
	}
}

void UModioCommonEmailAuthView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (BackButton)
		{
			ListenForInputAction(BackButton, UISettings->AuthParams.BackInputAction, UISettings->EmailAuthParams.BackButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				OnBackClicked.Broadcast();
			}));
		}

		if (SubmitButton)
		{
			ListenForInputAction(SubmitButton, UISettings->AuthParams.SubmitInputAction, UISettings->EmailAuthParams.SubmitButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				if (EmailTextBox) OnSubmitClicked.Broadcast(EmailTextBox->GetText().ToString());
			}));
		}

		if (CancelButton)
		{
			ListenForInputAction(CancelButton, UISettings->AuthParams.CancelInputAction, UISettings->EmailAuthParams.CancelButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
				OnCancelClicked.Broadcast();
			}));
		}

		if (EmailTextBox)
		{
			EmailTextBox->OnEditableTextChanged.AddDynamic(this, &UModioCommonEmailAuthView::OnEditableTextBoxTextChanged);
			SetValidationTextVisibility(ESlateVisibility::Hidden);
		}
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