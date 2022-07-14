#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"

void IModioUISubscriptionsChangedReceiver::SubscriptionsChangedHandler(FModioModID ModID, bool bNewSubscriptionState)
{
	bRoutedSubscriptionsChanged = false;
	NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	checkf(bRoutedSubscriptionsChanged,
		   TEXT("Please call IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged in your derived "
		   "implementation in order to route notifications to blueprint."));
}



void IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	bRoutedSubscriptionsChanged = true;
	Execute_OnSubscriptionsChanged(Cast<UObject>(this), ModID, bNewSubscriptionState);
}