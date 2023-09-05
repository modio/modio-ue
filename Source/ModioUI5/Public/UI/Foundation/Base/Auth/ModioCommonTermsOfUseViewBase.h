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
 * @brief Terms Of Use View Base that displays the terms of use and allows the user to accept or reject them
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonTermsOfUseViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnModioCommonTermsOfUseViewSubmitClicked);
	FOnModioCommonTermsOfUseViewSubmitClicked OnSubmitClicked;

	DECLARE_DELEGATE(FOnModioCommonTermsOfUseViewCancelClicked);
	FOnModioCommonTermsOfUseViewCancelClicked OnCancelClicked;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI")
	FModioTerms Terms;
};
