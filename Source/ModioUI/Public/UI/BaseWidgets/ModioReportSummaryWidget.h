// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModioMultiLineEditableTextBox.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Widget.h"
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
		switch (ReportType) {
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
