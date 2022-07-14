// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioReportButtonGroupWidget.h"

#include "Algo/Transform.h"
#include "Core/ModioReportInfoUI.h"
#include "IPlatformFilePak.h"
#include "ModioUISubsystem.h"
#include "UI/BaseWidgets/ModioWidgetBase.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/Dialogs/ModioDialogController.h"
#include "UI/Interfaces/IModioUIDialogButtonWidget.h"
#include "UI/Styles/ModioButtonStyle.h"
#include "Widgets/Input/SButton.h"

#include "Loc/BeginModioLocNamespace.h"

TSharedRef<SWidget> UModioReportButtonGroupWidget::RebuildWidget()
{
	UModioRichTextButton* CancelButton;
	CancelButton = NewObject<UModioRichTextButton>(this);
	CancelButton->SetButtonStyle(ButtonStyle);
	CancelButton->TakeWidget();
	CancelButton->SetLabel(LOCTEXT("Cancel", "Cancel"));
	CancelButton->OnClicked.AddDynamic(this, &UModioReportButtonGroupWidget::CancelClicked);

	// clang-format off
		SAssignNew(ButtonList, SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		[
			SNew(SGridPanel)
			+SGridPanel::Slot(0,0)
			.Padding(2)
			[
				CreateButton(EModioReportType::DMCA, LOCTEXT("DMCA", "DMCA"))
			]
			+SGridPanel::Slot(1,0)
			.Padding(2)
			[
				CreateButton(EModioReportType::NotWorking, LOCTEXT("NotWorking", "Not Working"))
			]
			+SGridPanel::Slot(0,1)
			.Padding(2)
			[
				CreateButton(EModioReportType::RudeContent, LOCTEXT("RudeContent", "Rude Content"))
			]
			+SGridPanel::Slot(1,1)
			.Padding(2) 
			[
				CreateButton(EModioReportType::IllegalContent, LOCTEXT("IllegalContent", "Illegal Content"))
			]
			+SGridPanel::Slot(0,2)
			.Padding(2)
			[
				CreateButton(EModioReportType::StolenContent, LOCTEXT("StolenContent", "Stolen Content"))
			]
			+SGridPanel::Slot(1,2)
			.Padding(2)
			[
				CreateButton(EModioReportType::FalseInformation, LOCTEXT("FalseInformation", "False Information"))
			]
			+SGridPanel::Slot(0,3)
			.Padding(2)
			[
				CreateButton(EModioReportType::Other, LOCTEXT("Other", "Other"))
			]
		]
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Padding(0, 16, 0, 0)
		[
			CancelButton->TakeWidget()
		];

	// clang-format on

	return ButtonList.ToSharedRef();
};

FReply UModioReportButtonGroupWidget::OnReportButtonClicked(EModioReportType ModioReport)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		UModioReportInfoUI* ReportDataSource = NewObject<UModioReportInfoUI>();

		if (DataSource)
		{
			if (DataSource->Implements<UModioModInfoUIDetails>())
			{
				ReportDataSource->ModInfo = DataSource;
				ReportDataSource->ReportData =
					FModioReportParams(IModioModInfoUIDetails::Execute_GetModID(DataSource), ModioReport);
				Subsystem->ShowReportEmailDialog(ReportDataSource);
			}
			else
			{
				UE_LOG(LogModioUI, Warning,
					   TEXT("Report button clicked with a Datasource that doesn't implement IModioModInfoUIDetails"));
			}
		}
		else
		{
			UE_LOG(LogModioUI, Warning, TEXT("Report button clicked with a null datasource"));
		}
	}

	return FReply::Handled();
}

void UModioReportButtonGroupWidget::CancelClicked()
{
	DialogController->FinalizeDialog();
}

TSharedRef<SButton> UModioReportButtonGroupWidget::CreateButton(EModioReportType ReportType, FText ButtonTitle)
{
	TSharedPtr<SButton> Button;

	// clang-format off
	SAssignNew(Button, SButton)
	.OnClicked_UObject(this, &UModioReportButtonGroupWidget::OnReportButtonClicked, ReportType)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	.ContentPadding(FMargin(16, 4,16, 2))
	[
		SNew(SModioRichTextBlock)
		.Text(ButtonTitle)
		.StyleReference_UObject(this, &UModioReportButtonGroupWidget::GetButtonTextStyle)
	];

	const FModioButtonStyle* ResolvedButtonStyle = ButtonStyle.FindStyle<FModioButtonStyle>();
	if (ResolvedButtonStyle)
	{
		Button->SetButtonStyle(&ResolvedButtonStyle->ButtonStyle);
		Button->SetContentPadding(ResolvedButtonStyle->ContentPadding);
	}
	Buttons.Add(Button);
	
	return Button.ToSharedRef();
}

void UModioReportButtonGroupWidget::NativeSetStyle(const FModioDialogStyle& Style)
{
	if (const FModioButtonStyle* ResolvedButtonStyle = Style.ButtonStyle.FindStyle<FModioButtonStyle>())
	{
		ButtonStyle = Style.ButtonStyle;

		for (TSharedPtr<SButton> CurrentButton : Buttons)
		{
			CurrentButton->SetButtonStyle(&ResolvedButtonStyle->ButtonStyle);
			CurrentButton->SetContentPadding(ResolvedButtonStyle->ContentPadding);

			CurrentButton->Invalidate(EInvalidateWidgetReason::Layout | EInvalidateWidgetReason::Paint);
		}
	}
}

const FModioUIStyleRef*UModioReportButtonGroupWidget::GetButtonTextStyle() const
{
	if (const FModioButtonStyle* ResolvedButtonStyle = ButtonStyle.FindStyle<FModioButtonStyle>())
	{
		return &ResolvedButtonStyle->RichTextStyle;
	}
	
	return &FModioButtonStyle::GetDefault().RichTextStyle;
}

#include "Loc/EndModioLocNamespace.h"