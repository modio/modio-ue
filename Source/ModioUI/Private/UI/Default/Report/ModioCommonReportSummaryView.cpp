/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/Report/ModioCommonReportSummaryView.h"

#include "Core/ModioModInfoUI.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/Text/MultiLineEditableTextBox/ModioCommonMultiLineEditableTextBox.h"
#include "UI/Settings/ModioCommonUISettings.h"
#include "UI/Settings/Params/ModioCommonReportParams.h"

void UModioCommonReportSummaryView::SetIsBusy(bool Busy)
{
	if (SubmitButton)
	{
		SubmitButton->SetIsEnabled(!Busy);
	}

	if (CancelButton)
	{
		CancelButton->SetIsEnabled(!Busy);
	}

	if (BackButton)
	{
		BackButton->SetIsEnabled(!Busy);
	}
}

// TODO: refactor this into a common
namespace
{
	FString ReportTypeToString(EModioReportType ReportType)
	{
		switch (ReportType) {
			case EModioReportType::Generic:
				return TEXT("Generic");
			case EModioReportType::DMCA:
				return TEXT("DMCA");
			case EModioReportType::NotWorking:
				return TEXT("Not Working");
			case EModioReportType::RudeContent:
				return TEXT("Rude Content");
			case EModioReportType::IllegalContent:
				return TEXT("Illegal Content");
			case EModioReportType::StolenContent:
				return TEXT("Stolen Content");
			case EModioReportType::FalseInformation:
				return TEXT("False Information");
			case EModioReportType::Other:
				return TEXT("Other");
			default: return TEXT("Unknown");
		}
	}
}

void UModioCommonReportSummaryView::SetReportParams_Implementation(const FModioReportParams& ReportParams)
{
	if (ReasonContentTextBlock)
	{
		ReasonContentTextBlock->SetText(FText::FromString(ReportTypeToString(ReportParams.Type)));
	}

	if (DetailsContentTextBlock)
	{
		DetailsContentTextBlock->SetText(FText::FromString(ReportParams.ReportDescription));
	}

	if (EmailContentTextBlock)
	{
		EmailContentTextBlock->SetText(FText::FromString(ReportParams.ReporterContact.Get(TEXT("Uknown"))));
	}
}

void UModioCommonReportSummaryView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (DescriptionTextBlock)
		{
			DescriptionTextBlock->SetText(UISettings->ReportSummaryParams.DescriptionText);
		}

		if (ReasonLabelTextBlock)
		{
			ReasonLabelTextBlock->SetText(UISettings->ReportSummaryParams.ReasonLabelText);
		}

		if (EmailLabelTextBlock)
		{
			EmailLabelTextBlock->SetText(UISettings->ReportSummaryParams.EmailLabelText);
		}

		if (DetailsLabelTextBlock)
		{
			DetailsLabelTextBlock->SetText(UISettings->ReportSummaryParams.DetailsLabelText);
		}

		if (BackButton)
		{
			BackButton->SetLabel(UISettings->ReportSummaryParams.BackButtonText);
		}

		if (SubmitButton)
		{
			SubmitButton->SetLabel(UISettings->ReportSummaryParams.SubmitButtonText);
		}

		if (CancelButton)
		{
			CancelButton->SetLabel(UISettings->ReportSummaryParams.CancelButtonText);
		}
	}
}

void UModioCommonReportSummaryView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (BackButton)
		{
			ListenForInputAction(BackButton, UISettings->ReportParams.BackInputAction, UISettings->ReportSummaryParams.BackButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{
				OnBackClicked.Broadcast();
			}));
		}

		if (SubmitButton)
		{
			ListenForInputAction(SubmitButton, UISettings->ReportParams.SubmitInputAction, UISettings->ReportSummaryParams.SubmitButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{
				OnSubmitClicked.Broadcast();
			}));
		}

		if (CancelButton)
		{
			ListenForInputAction(CancelButton, UISettings->ReportParams.CancelInputAction, UISettings->ReportSummaryParams.CancelButtonText, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [this]() 
			{
				OnCancelClicked.Broadcast();
			}));
		}
	}

	if (DetailsContentTextBlock)
	{
		DetailsContentTextBlock->SetIsReadOnly(true);
	}
}

void UModioCommonReportSummaryView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (UModioModInfoUI* ModInfoUI = Cast<UModioModInfoUI>(DataSource))
	{
		if (TitleTextBlock)
		{
			TitleTextBlock->SetText(FText::FromString(ModInfoUI->Underlying.ProfileName));
		}
	}
}

UWidget* UModioCommonReportSummaryView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	return BackButton;
}
