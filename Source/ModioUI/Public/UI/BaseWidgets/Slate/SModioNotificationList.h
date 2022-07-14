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