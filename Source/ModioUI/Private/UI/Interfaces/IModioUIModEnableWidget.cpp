#include "UI/Interfaces/IModioUIModEnableWidget.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"

void IModioUIModEnableWidget::ModEnabledStateChangedHandler(FModioModID ModID, bool bNewSubscriptionState)
{
	bRoutedModEnabledStateChanged = false;
	NativeOnModEnabledStateChanged(ModID, bNewSubscriptionState);
	checkf(bRoutedModEnabledStateChanged,
		   TEXT("Please call IModioUIModEnableWidget::NativeOnModEnabledStateChanged in your derived "
		   "implementation in order to route notifications to blueprint."));
}



void IModioUIModEnableWidget::NativeOnModEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	bRoutedModEnabledStateChanged = true;
	Execute_OnModEnabledStateChanged(Cast<UObject>(this), ModID, bNewSubscriptionState);
}

bool IModioUIModEnableWidget::QueryModEnabled(FModioModID ModID)
{
    if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
    {
        return Subsystem->QueryIsModEnabled(ModID);
    }
    else
    {
        return false;
    }
}

void IModioUIModEnableWidget::RequestModEnabledState(FModioModID ModID, bool bNewStateIsEnabled)
{
    if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
    {
        Subsystem->RequestModEnabledState(ModID, bNewStateIsEnabled);
    }
}