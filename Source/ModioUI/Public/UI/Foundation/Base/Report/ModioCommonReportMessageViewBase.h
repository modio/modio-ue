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
#include "ModioCommonReportMessageViewBase.generated.h"

/**
 * Report Message View that allows the user to enter a message to send a report to
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonReportMessageViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonReportMessageViewBackClicked);
	/**
	 * Fires when the user clicks the back button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonReportMessageViewBackClicked OnBackClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonReportMessageViewCancelClicked);
	/**
	 * Fires when the user clicks the cancel button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonReportMessageViewCancelClicked OnCancelClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModioCommonReportMessageViewSubmitClicked, const FString&, Message);
	/**
	 * Fires when the user clicks the submit button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonReportMessageViewSubmitClicked OnSubmitClicked;
};
