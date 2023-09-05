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
#include "InputCoreTypes.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Input/Reply.h"
#include "Layout/Visibility.h"
#include "Misc/Attribute.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Interfaces/IModioUINotification.h"
#include "UI/Notifications/ModioNotificationWidgetBase.h"

#include "Misc/EngineVersionComparison.h"
#if UE_VERSION_NEWER_THAN(5, 2, 0)
	#include "Engine/BlueprintGeneratedClass.h"
#endif

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
		if (NotificationList->GetChildrenCount() >= MaxNotificationsOnScreen)
		{
			NotificationList->RemoveChildAt(0);
		}
		FOnNotificationExpired ExpireDelegate;
		ExpireDelegate.BindDynamic(this, &UModioNotificationControllerBase::OnNotificationExpired);
		IModioUINotification::Execute_SetNotificationExpireHandler(UnderlyingWidget, ExpireDelegate);
		IModioUINotification::Execute_Display(UnderlyingWidget);
	}
}

void UModioNotificationControllerBase::NativeDisplayNotificationParams(const FModioNotificationParams& Params)
{
	if (GetWorld()->bIsTearingDown)
	{
		UE_LOG(LogTemp, Error, TEXT("Error during tearing down the world: %s"), *Params.ErrorCode.GetErrorMessage());
		return;
	}

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

void UModioNotificationControllerBase::NativeDisplayNotificationManual(const FText& title, const FText& message,
																			bool bIsError)
{
	if (GetWorld()->bIsTearingDown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Notification during tearing down the world: %s"), *message.ToString());
		return;
	}

	if (NotificationList)
	{
		UUserWidget* UnderlyingWidget = CreateWidget<UUserWidget>(this, ErrorNotificationClass);
		if (!UnderlyingWidget)
		{
			return;
		}
		IModioUINotification::Execute_ConfigureManual(UnderlyingWidget, title, message, bIsError);
		UVerticalBoxSlot* NewSlot = NotificationList->AddChildToVerticalBox(UnderlyingWidget);
		if (NotificationList->GetChildrenCount() >= MaxNotificationsOnScreen)
		{
			NotificationList->RemoveChildAt(0);
		}
		FOnNotificationExpired ExpireDelegate;
		ExpireDelegate.BindDynamic(this, &UModioNotificationControllerBase::OnNotificationExpired);
		IModioUINotification::Execute_SetNotificationExpireHandler(UnderlyingWidget, ExpireDelegate);
		IModioUINotification::Execute_Display(UnderlyingWidget);
		
	}
}
