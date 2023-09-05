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
#include "Types/ModioModManagementEvent.h"
#include "UObject/Interface.h"

#include "IModioUIModManagementEventReceiver.generated.h"

UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioUIModManagementEventReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIModManagementEventReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedModManagementEvent = false;
	void ModManagementEventHandler(FModioModManagementEvent Event);

protected:
	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->OnModManagementEvent.AddUObject(Cast<ImplementingClass>(this),
													   &IModioUIModManagementEventReceiver::ModManagementEventHandler);
		}
	}

	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event);

	UFUNCTION(BlueprintImplementableEvent)
	void OnModManagementEvent(FModioModManagementEvent Event);
};
