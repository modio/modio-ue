/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Notifications/ModioNotificationController.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Input/Reply.h"
#include "Layout/Visibility.h"
#include "Misc/Attribute.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Interfaces/IModioUINotification.h"
#include "UI/Notifications/ModioNotificationWidgetBase.h"

void UModioNotificationControllerBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUINotificationController::Register<UModioNotificationControllerBase>();
}

void UModioNotificationControllerBase::OnNotificationExpired(UWidget* Notification)
{
	if (NotificationList)
	{
		NotificationList->RemoveChild(Notification);
	}
}

void UModioNotificationControllerBase::NativeDisplayNotification(
	const TScriptInterface<IModioUINotification>& Notification)
{
	if (NotificationList)
	{
		UWidget* UnderlyingWidget = IModioUINotification::Execute_GetAsWidget(Notification.GetObject());
		UVerticalBoxSlot* NewSlot = NotificationList->AddChildToVerticalBox(UnderlyingWidget);
		FOnNotificationExpired ExpireDelegate;
		ExpireDelegate.BindDynamic(this, &UModioNotificationControllerBase::OnNotificationExpired);
		IModioUINotification::Execute_SetNotificationExpireHandler(UnderlyingWidget, ExpireDelegate);
		IModioUINotification::Execute_Display(UnderlyingWidget);
	}
}

void UModioNotificationControllerBase::NativeDisplayErrorNotification(const FModioNotificationParams& Params)
{
	if (*ErrorNotificationClass)
	{
		TSubclassOf<UWidget> ActualClass;
		if (UBlueprintGeneratedClass* GeneratedClass = Cast<UBlueprintGeneratedClass>(*ErrorNotificationClass))
		{
			if (GeneratedClass->ImplementsInterface(UModioUINotification::StaticClass()))
			{
				ActualClass = GeneratedClass;
			}
		}
		else if (ErrorNotificationClass->ImplementsInterface(UModioUINotification::StaticClass()))
		{
			ActualClass = ErrorNotificationClass.Get();
		}
		if (*ActualClass)
		{
			Execute_DisplayNotification(this,
										UModioNotificationWidgetBase::CreateFromParams(ActualClass, Params, this));
		}
	}
}
