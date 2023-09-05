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
#include "ModioCommonReportEmailViewBase.generated.h"

/**
 * @brief Report Email View that allows the user to enter an email address to send a report to
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonReportEmailViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnModioCommonReportEmailViewBackClicked);
	FOnModioCommonReportEmailViewBackClicked OnBackClicked;

	DECLARE_DELEGATE(FOnModioCommonReportEmailViewCancelClicked);
	FOnModioCommonReportEmailViewCancelClicked OnCancelClicked;

	DECLARE_DELEGATE_OneParam(FOnModioCommonReportEmailViewSubmitClicked, const FString& /*EmailAddress*/);
	FOnModioCommonReportEmailViewSubmitClicked OnSubmitClicked;
};
