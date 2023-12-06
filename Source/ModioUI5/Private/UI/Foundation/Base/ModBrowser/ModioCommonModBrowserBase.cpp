/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Base/ModBrowser/ModioCommonModBrowserBase.h"
#include "Core/ModioModInfoUI.h"
#include "UI/Foundation/Base/ModBrowser/ModioCommonModBrowserBaseStyle.h"

void UModioCommonModBrowserBase::SetStyle(TSubclassOf<UModioCommonModBrowserBaseStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonModBrowserBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUIUserChangedReceiver::Register<UModioCommonModBrowserBase>();
}

void UModioCommonModBrowserBase::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	bIsUserAuthenticated = NewUser.IsSet();
}

bool UModioCommonModBrowserBase::IsUserAuthenticated() const
{
	return bIsUserAuthenticated;
}
