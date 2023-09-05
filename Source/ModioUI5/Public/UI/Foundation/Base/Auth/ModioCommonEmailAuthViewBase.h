﻿/*
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
#include "ModioCommonEmailAuthViewBase.generated.h"

/**
 * @brief Email Auth View Base that displays a form for the user to enter their email address to begin the email auth process
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonEmailAuthViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnModioCommonEmailAuthViewBackClicked);
	FOnModioCommonEmailAuthViewBackClicked OnBackClicked;

	DECLARE_DELEGATE(FOnModioCommonEmailAuthViewCancelClicked);
	FOnModioCommonEmailAuthViewCancelClicked OnCancelClicked;

	DECLARE_DELEGATE_OneParam(FOnModioCommonEmailAuthViewSubmitClicked, const FString& /*EmailAddress*/);
	FOnModioCommonEmailAuthViewSubmitClicked OnSubmitClicked;
};
