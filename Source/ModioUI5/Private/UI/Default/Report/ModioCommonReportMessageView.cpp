/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/Report/ModioCommonReportMessageView.h"
#include "UI/Settings/Params/ModioCommonReportParams.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Text/MultiLineEditableTextBox/ModioCommonMultiLineEditableTextBox.h"
#include "Core/ModioModInfoUI.h"

void UModioCommonReportMessageView::SetValidationTextVisibility(ESlateVisibility Visbility)
{
	if (ValidationTextBlock)
	{
		ValidationTextBlock->SetVisibility(Visbility);
		CharactersTextBlock->SetVisibility(Visbility);
	}
}

void UModioCommonReportMessageView::SynchronizeProperties() 
{
	Super::SynchronizeProperties();

	if (const UModioCommonReportMessageParamsSettings* Settings = GetDefault<UModioCommonReportMessageParamsSettings>())
	{
		if (DescriptionTextBlock)
		{
			DescriptionTextBlock->SetText(Settings->DescriptionText);
		}

		if (ValidationTextBlock)
		{
			ValidationTextBlock->SetText(Settings->ValidationText);
			CharactersTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d characters remaining"), Settings->MessageLength)));
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

void UModioCommonReportMessageView::NativeOnInitialized() 
{
	Super::NativeOnInitialized();

	const UModioCommonReportParamsSettings* ReportSettings = GetDefault<UModioCommonReportParamsSettings>();
	const UModioCommonReportMessageParamsSettings* ReportMessageSettings = GetDefault<UModioCommonReportMessageParamsSettings>();
	if (!ReportSettings || !ReportMessageSettings)
	{
		return;
	}

	if (BackButton)
	{
		ListenForInputAction(BackButton, ReportSettings->BackInputAction, ReportMessageSettings->BackButtonText, [this]() 
		{
			if (OnBackClicked.IsBound()) OnBackClicked.Execute();
		});
	}

	if (SubmitButton)
	{
		ListenForInputAction(SubmitButton, ReportSettings->SubmitInputAction, ReportMessageSettings->SubmitButtonText, [this]() 
		{
			if (MessageTextBox)
			{
				if (OnSubmitClicked.IsBound()) OnSubmitClicked.Execute(MessageTextBox->GetText().ToString());
			}
		});
	}

	if (CancelButton)
	{
		ListenForInputAction(CancelButton, ReportSettings->CancelInputAction, ReportMessageSettings->CancelButtonText, [this]() 
		{
			if (OnCancelClicked.IsBound()) OnCancelClicked.Execute();
		});
	}

	if (MessageTextBox)
	{
		MessageTextBox->OnMultiLineEditableTextChanged.AddDynamic(this, &UModioCommonReportMessageView::OnMultiLineTextBoxTextChanged);
	}

	SetValidationTextVisibility(ESlateVisibility::Hidden);
}

void UModioCommonReportMessageView::NativeOnSetDataSource()
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

UWidget* UModioCommonReportMessageView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	return MessageTextBox.Get();
}

bool UModioCommonReportMessageView::IsMessageValid(const FString& Message)
{
	const UModioCommonReportMessageParamsSettings* Settings = GetDefault<UModioCommonReportMessageParamsSettings>();
	if (Settings)
	{
		if (Message.Len() > Settings->MessageLength)
		{
			return false;
		}
	}

	return true;
}

void UModioCommonReportMessageView::OnMultiLineTextBoxTextChanged(const FText& Text) 
{
	const UModioCommonReportMessageParamsSettings* Settings = GetDefault<UModioCommonReportMessageParamsSettings>();
	int remaining = Settings->MessageLength - Text.ToString().Len();
	int exceeded = Text.ToString().Len() - Settings->MessageLength;
	
	if (Text.ToString().Len() < Settings->MessageLength) 
	{
		SetValidationTextVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CharactersTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d characters exceeded."), exceeded)));
		SetValidationTextVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
