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
#include "CommonActivatableWidgetSwitcher.h"
#include "ModioCommonWidgetSwitcher.generated.h"

/**
 * @brief Widget Switcher that can be used in Mod.io UI
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonWidgetSwitcher : public UCommonActivatableWidgetSwitcher
{
	GENERATED_BODY()

protected:
	virtual void HandleSlateActiveIndexChanged(int32 ActiveIndex) override;
};
