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
#include "ModioCommonEmailAuthLoadingViewBase.generated.h"

/**
 * @brief Email Auth Code View Base that displays a loading state while the email auth process is ongoing
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonEmailAuthLoadingViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnModioCommonEmailAuthLoadingViewCancelClicked);
	FOnModioCommonEmailAuthLoadingViewCancelClicked OnCancelClicked;
};
