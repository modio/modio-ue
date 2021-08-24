#include "Types/ModioModManagementEvent.h"
#include "Internal/ModioConvert.h"

FModioModManagementEvent::FModioModManagementEvent(const Modio::ModManagementEvent& Event)
	: ID(ToUnreal(Event.ID)),
	  Event(ToUnreal(Event.Event)),
	  Status(ToUnreal(Event.Status))
{}
