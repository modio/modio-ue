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
#include "ModioCommonReportReasonViewBase.generated.h"

/**
 * Report Reason View that allows the user to select a reason for reporting a mod
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonReportReasonViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModioCommonReportReasonViewProceedClicked, EModioReportType, ReportType);
	/**
	 * Fires when the user clicks the proceed button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonReportReasonViewProceedClicked OnProceedClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonReportReasonViewCancelClicked);
	/**
	 * Fires when the user clicks the cancel button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonReportReasonViewCancelClicked OnCancelClicked;
};
