#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"

void IModioUIUserChangedReceiver::UserChangedHandler(TOptional<FModioUser> NewUser)
{
	bRoutedUserChanged = false;
	NativeUserChanged(NewUser);
	checkf(bRoutedUserChanged,
		   TEXT("Please call IModioUIUserChangedReceiver::NativeUserChanged in your derived "
		   "implementation in order to route notifications to blueprint."));
}



void IModioUIUserChangedReceiver::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	bRoutedUserChanged = true;
	Execute_OnUserChanged(Cast<UObject>(this), FModioOptionalUser {NewUser});
}