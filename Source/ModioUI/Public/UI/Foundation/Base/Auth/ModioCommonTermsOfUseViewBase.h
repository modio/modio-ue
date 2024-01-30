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
#include "Types/ModioTerms.h"
#include "ModioCommonTermsOfUseViewBase.generated.h"

/**
 * Terms Of Use View Base that displays the terms of use and allows the user to accept or reject them
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonTermsOfUseViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonTermsOfUseViewSubmitClicked);
	/**
	 * Fires when the user clicks the submit button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonTermsOfUseViewSubmitClicked OnSubmitClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModioCommonTermsOfUseViewCancelClicked);
	/**
	 * Fires when the user clicks the cancel button
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonTermsOfUseViewCancelClicked OnCancelClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModioCommonTermsOfUseLinkClicked, const FString&, URL);
	/**
	 * Fires when the user clicks a link
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio Common UI")
	FOnModioCommonTermsOfUseLinkClicked OnLinkClicked;

	/** Terms of use to display */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI")
	FModioTerms Terms;
};
