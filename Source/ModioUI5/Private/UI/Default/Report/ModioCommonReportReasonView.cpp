/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/Report/ModioCommonReportReasonView.h"
#include "UI/Settings/Params/ModioCommonReportParams.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/CheckBox/ModioCommonCheckBox.h"
#include "Core/ModioModInfoUI.h"

void UModioCommonReportReasonView::SynchronizeProperties() 
{
	Super::SynchronizeProperties();

	if (const UModioCommonReportReasonParamsSettings* Settings = GetDefault<UModioCommonReportReasonParamsSettings>())
	{
		if (DescriptionTextBlock)
		{
			DescriptionTextBlock->SetText(Settings->DescriptionText);
		}

		if (ProceedButton)
		{
			ProceedButton->SetLabel(Settings->ProceedButtonText);
		}

		if (CancelButton)
		{
			CancelButton->SetLabel(Settings->CancelButtonText);
		}
	}
}

void UModioCommonReportReasonView::NativeOnInitialized() 
{
	Super::NativeOnInitialized();
	
	const UModioCommonReportParamsSettings* ReportSettings = GetDefault<UModioCommonReportParamsSettings>();
	const UModioCommonReportReasonParamsSettings* ReportReasonSettings = GetDefault<UModioCommonReportReasonParamsSettings>();
	if (!ReportSettings || !ReportReasonSettings)
	{
		return;
	}

	if (ProceedButton)
	{
		ListenForInputAction(ProceedButton, ReportSettings->ProceedInputAction, ReportReasonSettings->ProceedButtonText, [this]() 
		{
			if (OnProceedClicked.IsBound()) OnProceedClicked.Execute(ReportType);
		});
	}

	if (CancelButton)
	{
		ListenForInputAction(CancelButton, ReportSettings->CancelInputAction, ReportReasonSettings->CancelButtonText, [this]() 
		{
			if (OnCancelClicked.IsBound()) OnCancelClicked.Execute();
		});
	}
	
	SetReportType(EModioReportType::DMCA);

	DMCACheckBox->OnModioCommonCheckBoxStateChanged.AddLambda([this](bool checked) 
	{
		SetReportType(EModioReportType::DMCA);
	});
	NotWorkingCheckBox->OnModioCommonCheckBoxStateChanged.AddLambda([this](bool checked) 
	{
		SetReportType(EModioReportType::NotWorking);
	});
	RudeContentCheckBox->OnModioCommonCheckBoxStateChanged.AddLambda([this](bool checked) 
	{
		SetReportType(EModioReportType::RudeContent);
	});
	IllegalContentCheckBox->OnModioCommonCheckBoxStateChanged.AddLambda([this](bool checked) 
	{
		SetReportType(EModioReportType::IllegalContent);
	});
	StolenContentCheckBox->OnModioCommonCheckBoxStateChanged.AddLambda([this](bool checked) 
	{
		SetReportType(EModioReportType::StolenContent);
	});
	FalseInformationCheckBox->OnModioCommonCheckBoxStateChanged.AddLambda([this](bool checked) 
	{
		SetReportType(EModioReportType::FalseInformation);
	});
	OtherCheckBox->OnModioCommonCheckBoxStateChanged.AddLambda([this](bool checked) 
	{
		SetReportType(EModioReportType::Other);
	});
}

void UModioCommonReportReasonView::NativeOnSetDataSource()
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

UWidget* UModioCommonReportReasonView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	return DMCACheckBox.Get();
}

void UModioCommonReportReasonView::SetAllCheckBoxesFalse() 
{
	DMCACheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	NotWorkingCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	RudeContentCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	IllegalContentCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	StolenContentCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	FalseInformationCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	OtherCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
}

void UModioCommonReportReasonView::SetReportType(EModioReportType Type) 
{
	ReportType = Type;

	switch (ReportType)
	{
		case EModioReportType::DMCA:
			SetAllCheckBoxesFalse();
			DMCACheckBox->SetCheckedState(ECheckBoxState::Checked);
			break;

		case EModioReportType::NotWorking:
			SetAllCheckBoxesFalse();
			NotWorkingCheckBox->SetCheckedState(ECheckBoxState::Checked);
			break;

		case EModioReportType::RudeContent:
			SetAllCheckBoxesFalse();
			RudeContentCheckBox->SetCheckedState(ECheckBoxState::Checked);
			break;

		case EModioReportType::IllegalContent:
			SetAllCheckBoxesFalse();
			IllegalContentCheckBox->SetCheckedState(ECheckBoxState::Checked);
			break;

		case EModioReportType::StolenContent:
			SetAllCheckBoxesFalse();
			StolenContentCheckBox->SetCheckedState(ECheckBoxState::Checked);
			break;

		case EModioReportType::FalseInformation:
			SetAllCheckBoxesFalse();
			FalseInformationCheckBox->SetCheckedState(ECheckBoxState::Checked);
			break;

		case EModioReportType::Other:
			SetAllCheckBoxesFalse();
			OtherCheckBox->SetCheckedState(ECheckBoxState::Checked);
			break;
	}
}
