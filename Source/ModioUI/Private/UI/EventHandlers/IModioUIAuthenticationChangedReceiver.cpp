#include "UI/EventHandlers/IModioUIAuthenticationChangedReceiver.h"

void IModioUIAuthenticationChangedReceiver::NativeOnAuthenticationChanged(TOptional<FModioUser> User)
{
	bRoutedAuthenticationChanged = true;
	Execute_OnAuthenticationChanged(Cast<UObject>(this), FModioOptionalUser {User});
}

void IModioUIAuthenticationChangedReceiver::UserAuthenticationChangedHandler(TOptional<FModioUser> User)
{
	bRoutedAuthenticationChanged = false;
	NativeOnAuthenticationChanged(User);
}
