// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioReportSummaryWidget.h"
#include "Core/ModioReportInfoUI.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Loc/BeginModioLocNamespace.h"

TSharedRef<SWidget> UModioReportSummaryWidget::RebuildWidget()
{
	Super::RebuildWidget();

	if (DataSource)
	{
		UModioReportInfoUI* ReportInfo = Cast<UModioReportInfoUI>(DataSource);

		if (ReportInfo)
		{
			FText ReportText = FText::Format(
				FText::FromString("{0}\n{1} \n\n{2}\n{3} \n\n{4}\n{5}"),
				LOCTEXT("ReasonForReporting", "Reason for Reporting"),
				FText::FromString(EnumToString(ReportInfo->ReportData.Type)),
				LOCTEXT("Email", "Email"),
				FText::FromString(ReportInfo->ReportData.ReporterContact.GetValue()),
				LOCTEXT("DetailsOfInfringement", "Details of Infringement"),
				FText::FromString(ReportInfo->ReportData.ReportDescription));

			SetText(ReportText);
		}
	}

	SetIsReadOnly(true);

	return MyEditableTextBlock.ToSharedRef();
}

#include "Loc/EndModioLocNamespace.h"