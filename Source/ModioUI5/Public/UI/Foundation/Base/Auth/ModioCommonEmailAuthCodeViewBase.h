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
 * @brief Email Auth Code View base that displays a view for entering an auth code
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonEmailAuthCodeViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnModioCommonEmailAuthCodeViewBackClicked);
	FOnModioCommonEmailAuthCodeViewBackClicked OnBackClicked;

	DECLARE_DELEGATE_OneParam(FOnModioCommonEmailAuthCodeViewSubmitClicked, const FString& /*AuthCode*/);
	FOnModioCommonEmailAuthCodeViewSubmitClicked OnSubmitClicked;

	DECLARE_DELEGATE(FOnModioCommonEmailAuthCodeViewCancelClicked);
	FOnModioCommonEmailAuthCodeViewCancelClicked OnCancelClicked;
};
