/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"

void IModioUIUserChangedReceiver::UserChangedHandler(TOptional<FModioUser> NewUser)
{
	bRoutedUserChanged = false;
	NativeUserChanged(NewUser);
	checkf(bRoutedUserChanged, TEXT("Please call IModioUIUserChangedReceiver::NativeUserChanged in your derived "
									"implementation in order to route notifications to blueprint."));
}

void IModioUIUserChangedReceiver::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	bRoutedUserChanged = true;
	Execute_OnUserChanged(Cast<UObject>(this), FModioOptionalUser {NewUser});
}