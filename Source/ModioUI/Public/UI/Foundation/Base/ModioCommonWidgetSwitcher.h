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
 * Widget Switcher that can be used in Mod.io UI
 * It provides some additional functionality over the base common activatable widget switcher
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonWidgetSwitcher : public UCommonActivatableWidgetSwitcher
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActiveIndexChanged_BP, UWidget*, InActiveWidget, int32, InActiveWidgetIndex);
	/**
	 * Fires when the active widget displayed by the switcher changes
	 */
	UPROPERTY(BlueprintAssignable, Category = "Mod.io Common UI|Widget Switcher")
	FOnActiveIndexChanged_BP OnActiveWidgetIndexChanged_BP;

protected:
	//~ Begin UCommonAnimatedSwitcher Interface
	virtual void HandleSlateActiveIndexChanged(int32 ActiveIndex) override;
	//~ End UCommonAnimatedSwitcher Interface
};
