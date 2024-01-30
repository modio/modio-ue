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
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/Interfaces/IModioModBrowser.h"
#include "ModioCommonModBrowserBase.generated.h"

class UModioCommonModBrowserBaseStyle;

/**
 * Mod Browser Base class. his is a main entry point for Mod.io UE5 UI
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModBrowserBase
	: public UModioCommonActivatableWidget,
	  public IModioModBrowserInterface,
	  public IModioUIUserChangedReceiver
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Mod Browser within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonModBrowserBaseStyle> InStyle);

protected:
	/** The style of the Mod Browser within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonModBrowserBaseStyle> ModioStyle;

protected:
	virtual void NativeOnInitialized() override;
	
	//~ Begin IModioUIUserChangedReceiver Interface
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	//~ End IModioUIUserChangedReceiver Interface

	/**
	 * Returns true if the user is authenticated
	 * @return True if the user is authenticated, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Info")
	bool IsUserAuthenticated() const;

private:
	/** Whether the user is authenticated or not */
	bool bIsUserAuthenticated = false;
};
