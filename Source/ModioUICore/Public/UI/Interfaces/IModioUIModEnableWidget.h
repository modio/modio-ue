/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "UObject/Interface.h"

#include "IModioUIModEnableWidget.generated.h"

UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioUIModEnableWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIModEnableWidget : public IInterface
{
	GENERATED_BODY()

	bool bRoutedModEnabledStateChanged = false;

public:
	UFUNCTION(BlueprintNativeEvent)
	void ModEnabledStateChangedHandler(FModioModID ModID, bool bNewSubscriptionState);
	virtual void ModEnabledStateChangedHandler_Implementation(FModioModID ModID, bool bNewSubscriptionState);

protected:
	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->OnModEnabledChanged.RemoveDynamic(Cast<ImplementingClass>(this), &IModioUIModEnableWidget::ModEnabledStateChangedHandler);
			Subsystem->OnModEnabledChanged.AddDynamic(Cast<ImplementingClass>(this), &IModioUIModEnableWidget::ModEnabledStateChangedHandler);
		}
	}

	virtual void NativeOnModEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnModEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState);

	virtual bool QueryModEnabled(FModioModID ModID);

	virtual void RequestModEnabledState(FModioModID ModID, bool bNewStateIsEnabled);
};
