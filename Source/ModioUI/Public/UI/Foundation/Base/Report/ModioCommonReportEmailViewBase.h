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
 * Report Email View that allows the user to enter an email address to send a report to
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonReportEmailViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonReportEmailViewBackClicked);
	/**
	 * Fires when the user clicks the back button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonReportEmailViewBackClicked OnBackClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonReportEmailViewCancelClicked);
	/**
	 * Fires when the user clicks the cancel button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonReportEmailViewCancelClicked OnCancelClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModioCommonReportEmailViewSubmitClicked, const FString&, EmailAddress);
	/**
	 * Fires when the user clicks the submit button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonReportEmailViewSubmitClicked OnSubmitClicked;

	/**
	 * Returns the validity state of the email address
	 * @param Email Email address to check
	 * @return Whether the email address is valid or not
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	bool IsEmailValid(const FString& Email);
	virtual bool IsEmailValid_Implementation(const FString& Email) { return true; }
};
