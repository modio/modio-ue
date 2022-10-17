/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Components/MultiLineEditableTextBox.h"
#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "ModioMultiLineEditableTextBox.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "UObject/Object.h"

#include "ModioReportSummaryWidget.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioReportSummaryWidget : public UModioMultiLineEditableTextBox, public IModioUIDataSourceWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UObject* DataSource;

protected:
	virtual void NativeSetDataSource(UObject* InDataSource) override
	{
		DataSource = InDataSource;
	}

	virtual void NativeSetHintText(FText InHintText) override {}

	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	FString EnumToString(EModioReportType ReportType)
	{
		switch (ReportType)
		{
			case EModioReportType::Generic:
				return FString("Generic");
			case EModioReportType::DMCA:
				return FString("DMCA");
			case EModioReportType::NotWorking:
				return FString("Not working");
			case EModioReportType::RudeContent:
				return FString("Rude content");
			case EModioReportType::IllegalContent:
				return FString("Illegal content");
			case EModioReportType::StolenContent:
				return FString("Stolen content");
			case EModioReportType::FalseInformation:
				return FString("False information");
			case EModioReportType::Other:
				return FString("Other");
			default:
				return FString();
		}
	}
};
