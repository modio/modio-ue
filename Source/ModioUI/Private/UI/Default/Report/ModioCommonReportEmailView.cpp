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
#include "UI/Settings/ModioCommonUISettings.h"

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

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (DescriptionTextBlock)
		{
			DescriptionTextBlock->SetText(UISettings->ReportEmailParams.DescriptionText);
		}

		if (ValidationTextBlock)
		{
			ValidationTextBlock->SetText(UISettings->ReportEmailParams.ValidationText);
		}

		if (BackButton)
		{
			BackButton->SetLabel(UISettings->ReportEmailParams.BackButtonText);
		}

		if (SubmitButton)
		{
			SubmitButton->SetLabel(UISettings->ReportEmailParams.SubmitButtonText);
		}

		if (CancelButton)
		{
			CancelButton->SetLabel(UISettings->ReportEmailParams.CancelButtonText);
		}
	}
}

void UModioCommonReportEmailView::NativeOnInitialized() 
{
	Super::NativeOnInitialized();
	
	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (BackButton)
		{
			ListenForInputAction(BackButton, UISettings->ReportParams.BackInputAction, UISettings->ReportEmailParams.BackButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{
				OnBackClicked.Broadcast();
			}));
		}

		if (SubmitButton)
		{
			ListenForInputAction(SubmitButton,  UISettings->ReportParams.SubmitInputAction, UISettings->ReportEmailParams.SubmitButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{
				OnSubmitClicked.Broadcast(EmailTextBox->GetText().ToString());
			}));
		}

		if (CancelButton)
		{
			ListenForInputAction(CancelButton,  UISettings->ReportParams.CancelInputAction, UISettings->ReportEmailParams.CancelButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{
				OnCancelClicked.Broadcast();
			}));
		}

		if (EmailTextBox)
		{
			EmailTextBox->OnEditableTextChanged.AddDynamic(this, &UModioCommonReportEmailView::OnEditableTextBoxTextChanged);
			SetValidationTextVisibility(ESlateVisibility::Hidden);
		}
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