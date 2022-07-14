#pragma once

#include "ModioUISubsystem.h"
#include "UObject/Interface.h"
#include "Core/Input/InputDeviceTypes.h"

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
