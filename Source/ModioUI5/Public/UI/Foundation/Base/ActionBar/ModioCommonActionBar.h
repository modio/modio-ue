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
#include "ModioCommonActionBar.generated.h"

class ULocalPlayer;

/**
 * @brief Base class for widgets that display a bottom action bar
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonActionBar : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void OnWidgetRebuilt() override;

	/**
	 * @brief Called when the widget is rebuilt to update the display
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	void HandleDeferredDisplayUpdate();

	/**
	 * @brief Returns the number of input actions that are currently bound to the action bar
	 * @return Number of input actions that are currently bound to the action bar
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	int32 GetNumInputActions() const;

	virtual void HandleBoundActionsUpdated(bool bFromOwningPlayer);
	virtual void HandlePlayerAdded(int32 PlayerIdx);
	virtual void MonitorPlayerActions(const ULocalPlayer* NewPlayer);
};
