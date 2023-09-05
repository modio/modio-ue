/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Base/QuickAccess/ModioCommonQuickAccessViewBase.h"

bool UModioCommonQuickAccessViewBase::IsUserAuthenticated() const
{
	return bIsUserAuthenticated;
}

void UModioCommonQuickAccessViewBase::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	bIsUserAuthenticated = NewUser.IsSet();
}

void UModioCommonQuickAccessViewBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUIUserChangedReceiver::Register<UModioCommonQuickAccessViewBase>();
}
