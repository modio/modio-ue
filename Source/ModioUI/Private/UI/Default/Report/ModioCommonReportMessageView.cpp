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
#include "UI/Settings/ModioCommonUISettings.h"

#include <Containers/UnrealString.h>

void UModioCommonReportMessageView::SetValidationTextVisibility(ESlateVisibility EVisibility)
{
	if (ValidationTextBlock)
	{
		ValidationTextBlock->SetVisibility(EVisibility);
		CharactersTextBlock->SetVisibility(EVisibility);
	}
}

void UModioCommonReportMessageView::SetErrorMessage(ESlateVisibility EVisibility) 
{
	if (ErrorMessageTextBlock)
	{
		ErrorMessageTextBlock->SetVisibility(EVisibility);
	}
}

void UModioCommonReportMessageView::SynchronizeProperties() 
{
	Super::SynchronizeProperties();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (DescriptionTextBlock)
		{
			DescriptionTextBlock->SetText(UISettings->ReportMessageParams.DescriptionText);
		}

		if (ValidationTextBlock)
		{
			ValidationTextBlock->SetText(UISettings->ReportMessageParams.ValidationText);
			CharactersTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d characters remaining"), UISettings->ReportMessageParams.MessageLength)));
		}

		if (BackButton)
		{
			BackButton->SetLabel(UISettings->ReportMessageParams.BackButtonText);
		}

		if (SubmitButton)
		{
			SubmitButton->SetLabel(UISettings->ReportMessageParams.SubmitButtonText);
		}

		if (CancelButton)
		{
			CancelButton->SetLabel(UISettings->ReportMessageParams.CancelButtonText);
		}
	}
}

void UModioCommonReportMessageView::NativeOnInitialized() 
{
	Super::NativeOnInitialized();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (BackButton)
		{
			ListenForInputAction(BackButton, UISettings->ReportParams.BackInputAction, UISettings->ReportMessageParams.BackButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{
				OnBackClicked.Broadcast();
			}));
		}

		if (SubmitButton)
		{
			ListenForInputAction(SubmitButton, UISettings->ReportParams.SubmitInputAction, UISettings->ReportMessageParams.SubmitButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{
				if (MessageTextBox)
				{
					OnSubmitClicked.Broadcast(MessageTextBox->GetText().ToString());
				}
			}));
		}

		if (CancelButton)
		{
			ListenForInputAction(CancelButton, UISettings->ReportParams.CancelInputAction, UISettings->ReportMessageParams.CancelButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{
				OnCancelClicked.Broadcast();
			}));
		}

		if (MessageTextBox)
		{
			MessageTextBox->OnMultiLineEditableTextChanged.AddDynamic(this, &UModioCommonReportMessageView::OnMultiLineTextBoxTextChanged);
		}

		SetValidationTextVisibility(ESlateVisibility::Hidden);
	}
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
	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{	
		FString CleanMessage = Message;
		CleanMessage.TrimStartAndEndInline();
		CleanMessage.ReplaceInline(TEXT("\n"), TEXT(""));
		CleanMessage.ReplaceInline(TEXT("\r"), TEXT(""));
		CleanMessage.ReplaceInline(TEXT("\t"), TEXT(""));
		if (CleanMessage.IsEmpty() || Message.Len() > UISettings->ReportMessageParams.MessageLength)
		{
			SetErrorMessage(ESlateVisibility::Visible);
			return false;
		}
	}
	SetErrorMessage(ESlateVisibility::Hidden);
	return true;
}

void UModioCommonReportMessageView::OnMultiLineTextBoxTextChanged(const FText& Text) 
{
	SetErrorMessage(ESlateVisibility::Hidden);

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		int32 Remaining = UISettings->ReportMessageParams.MessageLength - Text.ToString().Len();
		int32 Exceeded = Text.ToString().Len() - UISettings->ReportMessageParams.MessageLength;
	
		if (Text.ToString().Len() < UISettings->ReportMessageParams.MessageLength) 
		{
			SetValidationTextVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			CharactersTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d characters exceeded."), Exceeded)));
			SetValidationTextVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}
