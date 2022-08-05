#pragma once

#include "Types/ModioModManagementEvent.h"
#include "UObject/Interface.h"
#include "ModioUISubsystem.h"

#include "IModioUIModManagementEventReceiver.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIModManagementEventReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIModManagementEventReceiver : public IInterface
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
