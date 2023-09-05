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

	if (const UModioCommonReportSummaryParamsSettings* Settings = GetDefault<UModioCommonReportSummaryParamsSettings>())
	{
		if (DescriptionTextBlock)
		{
			DescriptionTextBlock->SetText(Settings->DescriptionText);
		}

		if (ReasonLabelTextBlock)
		{
			ReasonLabelTextBlock->SetText(Settings->ReasonLabelText);
		}

		if (EmailLabelTextBlock)
		{
			EmailLabelTextBlock->SetText(Settings->EmailLabelText);
		}

		if (DetailsLabelTextBlock)
		{
			DetailsLabelTextBlock->SetText(Settings->DetailsLabelText);
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

void UModioCommonReportSummaryView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const UModioCommonReportParamsSettings* ReportSettings = GetDefault<UModioCommonReportParamsSettings>();
	const UModioCommonReportSummaryParamsSettings* ReportSummarySettings = GetDefault<UModioCommonReportSummaryParamsSettings>();
	if (!ReportSettings || !ReportSummarySettings)
	{
		return;
	}

	if (BackButton)
	{
		ListenForInputAction(BackButton, ReportSettings->BackInputAction, ReportSummarySettings->BackButtonText, [this]() 
		{
			if (OnBackClicked.IsBound()) OnBackClicked.Execute();
		});
	}

	if (SubmitButton)
	{
		ListenForInputAction(SubmitButton, ReportSettings->SubmitInputAction, ReportSummarySettings->SubmitButtonText, [this]() 
		{
			if (OnSubmitClicked.IsBound()) OnSubmitClicked.Execute();
		});
	}

	if (CancelButton)
	{
		ListenForInputAction(CancelButton, ReportSettings->CancelInputAction, ReportSummarySettings->CancelButtonText, [this]() 
		{
			if (OnCancelClicked.IsBound()) OnCancelClicked.Execute();
		});
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
