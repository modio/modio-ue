/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Notifications/SNotificationList.h"

class SModioNotificationList : public SNotificationList
{
	friend class SModioNotificationItem;
	virtual void NotificationItemFadedOut(const TSharedRef<SNotificationItem>& NotificationItem) override;
	using FOnGenerateWidget = typename TSlateDelegates<UObject*>::FOnGenerateWidget;
	FOnGenerateWidget OnGenerateNotificationWidget;

public:
	SLATE_BEGIN_ARGS(SModioNotificationList) {}
	SLATE_EVENT(FOnGenerateWidget, OnGenerateNotificationWidget)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual TSharedRef<SNotificationItem> AddNotification(const FNotificationInfo& Info) override;
};