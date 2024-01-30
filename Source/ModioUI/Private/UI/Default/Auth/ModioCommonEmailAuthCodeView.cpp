/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/Auth/ModioCommonEmailAuthCodeView.h"

#include "ModioUI.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBox.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Settings/Params/ModioCommonAuthParams.h"
#include "Libraries/ModioSDKLibrary.h"
#include "UI/Foundation/Components/Text/CodeInputTextBox/ModioCommonCodeInputTextBox.h"
#include "UI/Settings/ModioCommonUISettings.h"

void UModioCommonEmailAuthCodeView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CodeInputTextBox)
	{
		CodeInputTextBox->OnCodeSubmit.AddUObject(this, &UModioCommonEmailAuthCodeView::HandleOnSubmitClicked);
	}

	if (CustomCodeInputTextBox)
	{
		CustomCodeInputTextBox->OnTextCommitted.AddDynamic(this, &UModioCommonEmailAuthCodeView::HandleOnCustomSubmitClicked);
	}
	
	const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>();
	if (!UISettings)
	{
		return;
	}

	if (BackButton)
	{
		ListenForInputAction(BackButton, UISettings->AuthParams.BackInputAction, UISettings->EmailAuthCodeParams.BackButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
			OnBackClicked.Broadcast();
		}));
	}

	if (SubmitButton)
	{
		ListenForInputAction(SubmitButton, UISettings->AuthParams.SubmitInputAction, UISettings->EmailAuthCodeParams.SubmitButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
			OnSubmitClicked.Broadcast(GetInputText());
		}));
	}

	if (CancelButton)
	{
		ListenForInputAction(CancelButton, UISettings->AuthParams.CancelInputAction, UISettings->EmailAuthCodeParams.CancelButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() {
			OnCancelClicked.Broadcast();
		}));
	}
}

void UModioCommonEmailAuthCodeView::HandleOnSubmitClicked_Implementation()
{
	OnSubmitClicked.Broadcast(GetInputText());
}

void UModioCommonEmailAuthCodeView::HandleOnCustomSubmitClicked_Implementation(const FText& Text, ETextCommit::Type CommitMethod)
{
	OnSubmitClicked.Broadcast(GetInputText());
}

void UModioCommonEmailAuthCodeView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (TitleTextBlock)
		{
			TitleTextBlock->SetText(UISettings->EmailAuthCodeParams.TitleText);
		}
		if (DescriptionTextBlock)
		{
			DescriptionTextBlock->SetText(UISettings->EmailAuthCodeParams.DescriptionText);
		}
		if (ValidationTextBlock)
		{
			ValidationTextBlock->SetText(UISettings->EmailAuthCodeParams.ValidationText);
		}
		if (BackButton)
		{
			BackButton->SetLabel(UISettings->EmailAuthCodeParams.BackButtonText);
		}
		if (SubmitButton)
		{
			SubmitButton->SetLabel(UISettings->EmailAuthCodeParams.SubmitButtonText);
		}
		if (CancelButton)
		{
			CancelButton->SetLabel(UISettings->EmailAuthCodeParams.CancelButtonText);
		}
	}
}

UWidget* UModioCommonEmailAuthCodeView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (CodeInputTextBox)
	{
		return CodeInputTextBox;
	}

	if (CustomCodeInputTextBox)
	{
		return CustomCodeInputTextBox;
	}

	return Super::NativeGetDesiredFocusTarget();
}

FString UModioCommonEmailAuthCodeView::GetInputText_Implementation() const
{
	if (CodeInputTextBox)
	{
		return CodeInputTextBox->GetCodeString();
	}
	if (CustomCodeInputTextBox)
	{
		return CustomCodeInputTextBox->GetText().ToString();
	}

	UE_LOG(ModioUI, Error, TEXT("Unable to get input text from UModioCommonEmailAuthCodeView: neither CodeInputTextBox nor CustomCodeInputTextBox are bound"));
	return FString();
}

void UModioCommonEmailAuthCodeView::SetValidationTextVisibility_Implementation(ESlateVisibility Visbility)
{
	if (ValidationTextBlock)
	{
		ValidationTextBlock->SetVisibility(Visbility);
	}
}

bool UModioCommonEmailAuthCodeView::IsCodeValid_Implementation(const FString& Code)
{
	if (!UModioSDKLibrary::IsValidSecurityCodeFormat(Code))
	{
		SetValidationTextVisibility(ESlateVisibility::Visible);
		return false;
	}

	SetValidationTextVisibility(ESlateVisibility::Hidden);
	return true;
}

void UModioCommonEmailAuthCodeView::SetIsBusy_Implementation(bool bBusy) 
{
	if (SubmitButton)
	{
		SubmitButton->SetIsEnabled(!bBusy);
	}
	if (CancelButton)
	{
		CancelButton->SetIsEnabled(!bBusy);
	}
	if (BackButton)
	{
		BackButton->SetIsEnabled(!bBusy);
	}
}
