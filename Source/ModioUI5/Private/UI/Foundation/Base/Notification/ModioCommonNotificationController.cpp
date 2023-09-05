/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Base/Notification/ModioCommonNotificationController.h"

#include "Components/PanelWidget.h"
#include "UI/Foundation/Base/Notification/ModioCommonNotificationWidgetBase.h"
#include "Engine/BlueprintGeneratedClass.h"

void UModioCommonNotificationController::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUINotificationController::Register<UModioCommonNotificationController>();
}

void UModioCommonNotificationController::OnNotificationExpired(UWidget* Notification)
{
	if (NotificationList)
	{
		NotificationList->RemoveChild(Notification);
	}
}

void UModioCommonNotificationController::NativeDisplayNotification(const TScriptInterface<IModioUINotification>& Notification)
{
	IModioUINotificationController::NativeDisplayNotification(Notification);
	if (NotificationList)
	{
		UWidget* UnderlyingWidget = IModioUINotification::Execute_GetAsWidget(Notification.GetObject());
		UE_LOG(LogTemp, Log, TEXT("Displaying notification %s"), *UnderlyingWidget->GetName());
		NotificationList->AddChild(UnderlyingWidget);
		if (NotificationList->GetChildrenCount() >= MaxNotificationsOnScreen)
		{
			NotificationList->RemoveChildAt(0);
		}
		FOnNotificationExpired ExpireDelegate;
		ExpireDelegate.BindDynamic(this, &UModioCommonNotificationController::OnNotificationExpired);
		IModioUINotification::Execute_SetNotificationExpireHandler(UnderlyingWidget, ExpireDelegate);
		IModioUINotification::Execute_Display(UnderlyingWidget);
	}
}

void UModioCommonNotificationController::NativeDisplayNotificationParams(const FModioNotificationParams& Params)
{
	IModioUINotificationController::NativeDisplayNotificationParams(Params);
	if (ErrorNotificationClass && SuccessNotificationClass)
	{
		TSubclassOf<UWidget> ActualClass;
		if (UBlueprintGeneratedClass* GeneratedClass = Cast<UBlueprintGeneratedClass>(Params.ErrorCode ? ErrorNotificationClass.Get() : SuccessNotificationClass.Get()))
		{
			if (GeneratedClass->ImplementsInterface(UModioUINotification::StaticClass()))
			{
				ActualClass = GeneratedClass;
			}
		}
		else if (ErrorNotificationClass->ImplementsInterface(UModioUINotification::StaticClass()) && SuccessNotificationClass->ImplementsInterface(UModioUINotification::StaticClass()))
		{
			ActualClass = Params.ErrorCode ? ErrorNotificationClass.Get() : SuccessNotificationClass.Get();
		}
		if (*ActualClass)
		{
			Execute_DisplayNotification(this, UModioCommonNotificationWidgetBase::CreateFromParams(ActualClass, Params, this));
		}
	}
}

void UModioCommonNotificationController::NativeDisplayNotificationManual(const FText& Title, const FText& Message, bool bIsError)
{
	IModioUINotificationController::NativeDisplayNotificationManual(Title, Message, bIsError);
	if (NotificationList && ErrorNotificationClass && SuccessNotificationClass && GetWorld() && !GetWorld()->bIsTearingDown)
	{
		UUserWidget* UnderlyingWidget = CreateWidget<UUserWidget>(this, bIsError ? ErrorNotificationClass : SuccessNotificationClass);
		if (!UnderlyingWidget)
		{
			return;
		}
		IModioUINotification::Execute_ConfigureManual(UnderlyingWidget, Title, Message, bIsError);
		NotificationList->AddChild(UnderlyingWidget);
		if (NotificationList->GetChildrenCount() >= MaxNotificationsOnScreen)
		{
			NotificationList->RemoveChildAt(0);
		}
		FOnNotificationExpired ExpireDelegate;
		ExpireDelegate.BindDynamic(this, &UModioCommonNotificationController::OnNotificationExpired);
		IModioUINotification::Execute_SetNotificationExpireHandler(UnderlyingWidget, ExpireDelegate);
		IModioUINotification::Execute_Display(UnderlyingWidget);
	}
}
