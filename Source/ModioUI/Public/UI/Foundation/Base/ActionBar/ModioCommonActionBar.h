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
 * Base class for widgets that display a bottom action bar
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonActionBar : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	//~ Begin UWidget Interface
	virtual void OnWidgetRebuilt() override;
	//~ End UWidget Interface

	/**
	 * Called when the widget is rebuilt to update the display
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	void HandleDeferredDisplayUpdate();

public:
	/**
	 * Returns the number of input actions that are currently bound to the action bar
	 * @return Number of input actions that are currently bound to the action bar
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	int32 GetNumInputActions() const;

protected:
	/**
	 * Called when the bound actions are updated
	 * @param bFromOwningPlayer True if the bound actions were updated from the owning player
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleBoundActionsUpdated(bool bFromOwningPlayer);

	/**
	 * Called when a player is added to the game instance
	 * @param PlayerIdx Index of the player that was added
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandlePlayerAdded(int32 PlayerIdx);

	/**
	 * Called when a player is removed from the game instance
	 * @param NewPlayer Player that was removed
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void MonitorPlayerActions(const ULocalPlayer* NewPlayer);
};
