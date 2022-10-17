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

#include "Core/Input/InputDeviceTypes.h"
#include "ModioUISubsystem.h"
#include "UObject/Interface.h"

#include "IModioUIInputDeviceChangedReceiver.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIInputDeviceChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIInputDeviceChangedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedInputDeviceChanged = false;
	void InputDeviceChangedHandler(EModioUIInputMode InputDevice);

protected:
	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->OnInputDeviceChanged.AddUObject(Cast<ImplementingClass>(this),
													   &IModioUIInputDeviceChangedReceiver::InputDeviceChangedHandler);

			NativeOnInputDeviceChanged(Subsystem->GetLastInputDevice());
		}
	};

	virtual void NativeOnInputDeviceChanged(EModioUIInputMode InputDevice);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInputDeviceChanged(EModioUIInputMode InputDevice);
};
