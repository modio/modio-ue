/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "Types/ModioReportParams.h"
#include "ModioCommonReportSummaryViewBase.generated.h"

/**
 * @brief Report Summary View that shows the user a summary of their report
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonReportSummaryViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnModioCommonReportSummaryViewBackClicked);
	FOnModioCommonReportSummaryViewBackClicked OnBackClicked;

	DECLARE_DELEGATE(FOnModioCommonReportSummaryViewCancelClicked);
	FOnModioCommonReportSummaryViewCancelClicked OnCancelClicked;

	DECLARE_DELEGATE(FOnModioCommonReportSummaryViewSubmitClicked);
	FOnModioCommonReportSummaryViewSubmitClicked OnSubmitClicked;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetReportParams(const FModioReportParams& ReportParams);
	virtual void SetReportParams_Implementation(const FModioReportParams& ReportParams) {}
};
