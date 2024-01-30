/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Base/Notification/ModioCommonNotificationWidgetBase.h"

#include "ModioUISubsystem.h"
#include "Loc/BeginModioLocNamespace.h"

UWidget* UModioCommonNotificationWidgetBase::NativeGetAsWidget()
{
	return Cast<UWidget>(this);
}

void UModioCommonNotificationWidgetBase::NativeSetNotificationExpireHandler(const FOnNotificationExpired& InDelegate)
{
	IModioUINotification::NativeSetNotificationExpireHandler(InDelegate);
	ExpireDelegate = InDelegate;
}

void UModioCommonNotificationWidgetBase::NativeConfigure(const FModioNotificationParams& Params)
{
	IModioUINotification::NativeConfigure(Params);
	if (!FMath::IsNearlyZero(Params.Duration))
	{
		ExpiryDuration = Params.Duration;
	}
}

void UModioCommonNotificationWidgetBase::NativeConfigureManual(const FText& Title, const FText& Message, bool bIsError)
{
	IModioUINotification::NativeConfigureManual(Title, Message, bIsError);
	ExpiryDuration = 1.0f;
}

void UModioCommonNotificationWidgetBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
#if WITH_EDITOR
	if (IsDesignTime())
	{
		IModioUINotification::Execute_Configure(this, PreviewParameters);
	}
#endif
}

#include "Loc/EndModioLocNamespace.h"