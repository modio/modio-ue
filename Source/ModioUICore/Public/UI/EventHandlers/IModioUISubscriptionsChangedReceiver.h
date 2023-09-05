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

#include "IModioUISubscriptionsChangedReceiver.generated.h"

UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioUISubscriptionsChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUISubscriptionsChangedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedSubscriptionsChanged = false;
	void SubscriptionsChangedHandler(FModioModID ModID, bool bNewSubscriptionState);

protected:
	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->OnSubscriptionStatusChanged.AddUObject(
				Cast<ImplementingClass>(this), &IModioUISubscriptionsChangedReceiver::SubscriptionsChangedHandler);
		}
	}

	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState);
};
