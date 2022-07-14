#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"

void IModioUIModManagementEventReceiver::ModManagementEventHandler(FModioModManagementEvent Event)
{
	bRoutedModManagementEvent = false;
	NativeOnModManagementEvent(Event);
	checkf(bRoutedModManagementEvent,
		   TEXT("Please call IModioUIModManagementEventReceiver::NativeOnModManagementEvent in your derived "
				"implementation in order to route notifications to blueprint"));
}

void IModioUIModManagementEventReceiver::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	bRoutedModManagementEvent = true;
	Execute_OnModManagementEvent(Cast<UObject>(this), Event);
}