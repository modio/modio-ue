#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"

void IModioUIModManagementEventReceiver::ModManagementEventHandler(FModioModManagementEvent Event)
{
	bRoutedModManagementEvent = false;
	TRACE_CPUPROFILER_EVENT_SCOPE_TEXT(*(Cast<UObject>(this)->GetFName().ToString().Append("_ManagementEventHandler")));
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