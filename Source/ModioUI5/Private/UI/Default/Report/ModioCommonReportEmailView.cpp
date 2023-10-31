/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/Report/ModioCommonReportEmailView.h"
#include "UI/Settings/Params/ModioCommonReportParams.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Text/EditableTextBox/ModioCommonEditableTextBox.h"
#include "Core/ModioModInfoUI.h"
#include "Libraries/ModioSDKLibrary.h"

void UModioCommonReportEmailView::SetValidationTextVisibility_Implementation(ESlateVisibility EVisbility) 
{
	if (ValidationTextBlock) 
	{
		ValidationTextBlock->SetVisibility(EVisbility);
	}
}

void UModioCommonReportEmailView::SynchronizeProperties() 
{
	Super::SynchronizeProperties();

	if (const UModioCommonReportEmailParamsSettings* Settings = GetDefault<UModioCommonReportEmailParamsSettings>())
	{
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

void UModioCommonReportEmailView::NativeOnInitialized() 
{
	Super::NativeOnInitialized();
	
	const UModioCommonReportParamsSettings* ReportSettings = GetDefault<UModioCommonReportParamsSettings>();
	const UModioCommonReportEmailParamsSettings* ReportEmailSettings = GetDefault<UModioCommonReportEmailParamsSettings>();
	if (!ReportSettings || !ReportEmailSettings)
	{
		return;
	}

	if (BackButton)
	{
		ListenForInputAction(BackButton, ReportSettings->BackInputAction, ReportEmailSettings->BackButtonText, [this]() 
		{
			if (OnBackClicked.IsBound()) OnBackClicked.Execute();
		});
	}

	if (SubmitButton)
	{
		ListenForInputAction(SubmitButton, ReportSettings->SubmitInputAction, ReportEmailSettings->SubmitButtonText, [this]() 
		{
			if (OnSubmitClicked.IsBound()) OnSubmitClicked.Execute(EmailTextBox->GetText().ToString());
		});
	}

	if (CancelButton)
	{
		ListenForInputAction(CancelButton, ReportSettings->CancelInputAction, ReportEmailSettings->CancelButtonText, [this]() 
		{
			if (OnCancelClicked.IsBound()) OnCancelClicked.Execute();
		});
	}

	if (EmailTextBox)
	{
		EmailTextBox->OnEditableTextChanged.AddDynamic(this, &UModioCommonReportEmailView::OnEditableTextBoxTextChanged);
		SetValidationTextVisibility(ESlateVisibility::Hidden);
	}
}

void UModioCommonReportEmailView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	UModioModInfoUI* ModInfoUI = Cast<UModioModInfoUI>(DataSource);
	if (ModInfoUI)
	{
		if (TitleTextBlock)
		{
			TitleTextBlock->SetText(FText::FromString(ModInfoUI->Underlying.ProfileName));
		}
	}
}

UWidget* UModioCommonReportEmailView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	return EmailTextBox.Get();
}

bool UModioCommonReportEmailView::IsEmailValid_Implementation(const FString& Email)
{
	if (!UModioSDKLibrary::IsValidEmailAddressFormat(Email))
	{
		SetValidationTextVisibility(ESlateVisibility::Visible);
		return false;
	}
	
	SetValidationTextVisibility(ESlateVisibility::Hidden);
	return true;
}

void UModioCommonReportEmailView::OnEditableTextBoxTextChanged_Implementation(const FText& InText) 
{
	SetValidationTextVisibility(ESlateVisibility::Hidden);
}