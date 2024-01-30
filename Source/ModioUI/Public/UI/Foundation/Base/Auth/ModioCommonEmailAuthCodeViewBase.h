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
#include "ModioCommonEmailAuthCodeViewBase.generated.h"

/**
 * Email Auth Code View base that displays a view for entering an auth code
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonEmailAuthCodeViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonEmailAuthCodeViewBackClicked);
	/**
	 * Fires when the user clicks the back button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonEmailAuthCodeViewBackClicked OnBackClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModioCommonEmailAuthCodeViewSubmitClicked, const FString&, AuthCode);
	/**
	 * Fires when the user clicks the submit button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonEmailAuthCodeViewSubmitClicked OnSubmitClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonEmailAuthCodeViewCancelClicked);
	/**
	 * Fires when the user clicks the cancel button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonEmailAuthCodeViewCancelClicked OnCancelClicked;
};
