﻿/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/Auth/ModioCommonEmailAuthCodeView.h"

#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBox.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Settings/Params/ModioCommonAuthParams.h"
#include "Libraries/ModioSDKLibrary.h"

void UModioCommonEmailAuthCodeView::SetValidationTextVisibility(ESlateVisibility Visbility)
{
	if (ValidationTextBlock)
	{
		ValidationTextBlock->SetVisibility(Visbility);
	}
}

void UModioCommonEmailAuthCodeView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// TODO: Redesign this
	if (DigitTextBox1)
	{
		DigitTextBox1->OnTextChanged.AddDynamic(this, &UModioCommonEmailAuthCodeView::OnTextBox1Changed);
		DigitTextBox2->OnTextChanged.AddDynamic(this, &UModioCommonEmailAuthCodeView::OnTextBox2Changed);
		DigitTextBox3->OnTextChanged.AddDynamic(this, &UModioCommonEmailAuthCodeView::OnTextBox3Changed);
		DigitTextBox4->OnTextChanged.AddDynamic(this, &UModioCommonEmailAuthCodeView::OnTextBox4Changed);
		DigitTextBox5->OnTextChanged.AddDynamic(this, &UModioCommonEmailAuthCodeView::OnTextBox5Changed);
	}

	const UModioCommonAuthParamsSettings* AuthSettings = GetDefault<UModioCommonAuthParamsSettings>();
	const UModioCommonEmailAuthCodeParamsSettings* AuthCodeSettings = GetDefault<UModioCommonEmailAuthCodeParamsSettings>();
	if (!AuthSettings || !AuthCodeSettings)
	{
		return;
	}

	if (BackButton)
	{
		ListenForInputAction(BackButton, AuthSettings->BackInputAction, AuthCodeSettings->BackButtonText, [this]() {
			if (OnBackClicked.IsBound()) OnBackClicked.Execute();
		});
	}

	if (SubmitButton)
	{
		ListenForInputAction(SubmitButton, AuthSettings->SubmitInputAction, AuthCodeSettings->SubmitButtonText, [this]() {
			if (OnSubmitClicked.IsBound()) OnSubmitClicked.Execute(GetInputText());
		});
	}

	if (CancelButton)
	{
		ListenForInputAction(CancelButton, AuthSettings->CancelInputAction, AuthCodeSettings->CancelButtonText, [this]() {
			if (OnCancelClicked.IsBound()) OnCancelClicked.Execute();
		});
	}
}

void UModioCommonEmailAuthCodeView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonEmailAuthCodeParamsSettings* Settings = GetDefault<UModioCommonEmailAuthCodeParamsSettings>())
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

UWidget* UModioCommonEmailAuthCodeView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (DigitTextBox1->HasAnyUserFocus()) 
	{
		return DigitTextBox1;
	}
	else if (DigitTextBox2->HasAnyUserFocus())
	{
		return DigitTextBox2;
	}
	else if (DigitTextBox3->HasAnyUserFocus())
	{
		return DigitTextBox3;
	}
	else if (DigitTextBox4->HasAnyUserFocus())
	{
		return DigitTextBox4;
	}
	else if (DigitTextBox5->HasAnyUserFocus())
	{
		return DigitTextBox5;
	}

	return DigitTextBox1;
}

void UModioCommonEmailAuthCodeView::OnTextBox1Changed(const FText& Text) 
{
	HandleInputText(Text, DigitTextBox1);
}

void UModioCommonEmailAuthCodeView::OnTextBox2Changed(const FText& Text)
{
	HandleInputText(Text, DigitTextBox2);
}

void UModioCommonEmailAuthCodeView::OnTextBox3Changed(const FText& Text)
{
	HandleInputText(Text, DigitTextBox3);
}

void UModioCommonEmailAuthCodeView::OnTextBox4Changed(const FText& Text)
{
	HandleInputText(Text, DigitTextBox4);
}

void UModioCommonEmailAuthCodeView::OnTextBox5Changed(const FText& Text)
{
	HandleInputText(Text, DigitTextBox5);
}

void UModioCommonEmailAuthCodeView::HandleInputText(const FText& Text, UModioCommonEditableTextBox* TextBox)
{
	SetValidationTextVisibility(ESlateVisibility::Hidden);
	
	// If a user wants to remove all 5 digits after typing them all incorrectly, Erasing the digit (Backspace) will take them one step previous.
	if (Text.IsEmpty())
	{
		if (TextBox == DigitTextBox5)
		{
			DigitTextBox4->SetKeyboardFocus();
		}
		else if (TextBox == DigitTextBox4)
		{
			DigitTextBox3->SetKeyboardFocus();
		}
		else if (TextBox == DigitTextBox3)
		{
			DigitTextBox2->SetKeyboardFocus();
		}
		else if (TextBox == DigitTextBox2)
		{
			DigitTextBox1->SetKeyboardFocus();
		}
		return;
	}

	// If text is of 5 digits in length, it means the user has pasted all 5 digits in one go so copy each digit.
	if (Text.ToString().Len() == 5)
	{
		DigitTextBox1->SetText(FText::FromString(Text.ToString().Mid(0, 1)));
		DigitTextBox2->SetText(FText::FromString(Text.ToString().Mid(1, 1)));
		DigitTextBox3->SetText(FText::FromString(Text.ToString().Mid(2, 1)));
		DigitTextBox4->SetText(FText::FromString(Text.ToString().Mid(3, 1)));
		DigitTextBox5->SetText(FText::FromString(Text.ToString().Mid(4, 1)));
	}
	
	// The user has typed a character.
	else
	{
		// Don't let the user enter more than a single character per box.
		if (Text.ToString().Len() >= 1)
		{
			TextBox->SetText(FText::FromString(Text.ToString().Mid(0, 1)));
		}

		// Transfer the focus to the next box to make it easier for the user to input quickly.
		if (TextBox == DigitTextBox1)
		{
			DigitTextBox2->SetKeyboardFocus();
		}
		else if (TextBox == DigitTextBox2)
		{
			DigitTextBox3->SetKeyboardFocus();
		}
		else if (TextBox == DigitTextBox3)
		{
			DigitTextBox4->SetKeyboardFocus();
		}
		else if (TextBox == DigitTextBox4)
		{
			DigitTextBox5->SetKeyboardFocus();
		}
	}
}

FString UModioCommonEmailAuthCodeView::GetInputText() const
{
	FString InputText;
	if (DigitTextBox1)
	{
		InputText.Append(DigitTextBox1->GetText().ToString());
	}
	if (DigitTextBox2)
	{
		InputText.Append(DigitTextBox2->GetText().ToString());
	}
	if (DigitTextBox3)
	{
		InputText.Append(DigitTextBox3->GetText().ToString());
	}
	if (DigitTextBox4)
	{
		InputText.Append(DigitTextBox4->GetText().ToString());
	}
	if (DigitTextBox5)
	{
		InputText.Append(DigitTextBox5->GetText().ToString());
	}
	return InputText;
}

bool UModioCommonEmailAuthCodeView::IsCodeValid(const FString& Code)
{
	if (!UModioSDKLibrary::IsValidSecurityCodeFormat(Code))
	{
		SetValidationTextVisibility(ESlateVisibility::Visible);
		return false;
	}

	SetValidationTextVisibility(ESlateVisibility::Hidden);
	return true;
}

void UModioCommonEmailAuthCodeView::SetIsBusy(bool Busy) 
{
	SubmitButton->SetIsEnabled(!Busy);
	CancelButton->SetIsEnabled(!Busy);
	BackButton->SetIsEnabled(!Busy);
}
