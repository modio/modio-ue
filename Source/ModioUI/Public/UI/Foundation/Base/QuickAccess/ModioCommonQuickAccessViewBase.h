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
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "ModioCommonQuickAccessViewBase.generated.h"

/**
 * Quick Access View Base that can be used to display a list of quick access items
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonQuickAccessViewBase
	: public UModioCommonActivatableWidget,
	  public IModioUIUserChangedReceiver
{
	GENERATED_BODY()

public:
	/**
	 * Returns true if the user is authenticated
	 * @return True if the user is authenticated, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Info")
	bool IsUserAuthenticated() const;

protected:
	//~ Begin IModioUIUserChangedReceiver Interface
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	//~ End IModioUIUserChangedReceiver Interface
private:
	/** Whether the user is authenticated or not */
	bool bIsUserAuthenticated = false;

protected:
	//~ Begin UModioCommonActivatableWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UModioCommonActivatableWidget Interface
};
