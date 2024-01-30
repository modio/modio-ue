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
 * Report Summary View that shows the user a summary of their report
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonReportSummaryViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonReportSummaryViewBackClicked);
	/**
	 * Fires when the user clicks the back button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonReportSummaryViewBackClicked OnBackClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonReportSummaryViewCancelClicked);
	/**
	 * Fires when the user clicks the cancel button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonReportSummaryViewCancelClicked OnCancelClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonReportSummaryViewSubmitClicked);
	/**
	 * Fires when the user clicks the submit button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonReportSummaryViewSubmitClicked OnSubmitClicked;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetReportParams(const FModioReportParams& ReportParams);
	virtual void SetReportParams_Implementation(const FModioReportParams& ReportParams) {}
};
