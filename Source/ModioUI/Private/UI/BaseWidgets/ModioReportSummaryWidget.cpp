/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioReportSummaryWidget.h"
#include "Core/ModioReportInfoUI.h"
#include "Loc/BeginModioLocNamespace.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SBox.h"

TSharedRef<SWidget> UModioReportSummaryWidget::RebuildWidget()
{
	Super::RebuildWidget();

	if (DataSource)
	{
		UModioReportInfoUI* ReportInfo = Cast<UModioReportInfoUI>(DataSource);

		if (ReportInfo)
		{
			FText ReportText =
				FText::Format(FText::FromString("{0}\n{1} \n\n{2}\n{3} \n\n{4}\n{5}"),
							  LOCTEXT("ReasonForReporting", "Reason for Reporting"),
							  FText::FromString(EnumToString(ReportInfo->ReportData.Type)), LOCTEXT("Email", "Email"),
							  FText::FromString(ReportInfo->ReportData.ReporterContact.GetValue()),
							  LOCTEXT("DetailsOfInfringement", "Details of Infringement"),
							  FText::FromString(ReportInfo->ReportData.ReportDescription));
			WrappingPolicy = ETextWrappingPolicy::AllowPerCharacterWrapping;
			SetText(ReportText);
		}
	}

	SetIsReadOnly(true);
	
	return SNew(SBox).WidthOverride(700.0f).VAlign(VAlign_Fill).Content()[MyEditableTextBlock.ToSharedRef()];
}

#include "Loc/EndModioLocNamespace.h"