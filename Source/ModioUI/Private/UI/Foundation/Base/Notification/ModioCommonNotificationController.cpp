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

#include "ModioUI.h"
#include "Components/PanelWidget.h"
#include "UI/Foundation/Base/Notification/ModioCommonNotificationWidgetBase.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Libraries/ModioErrorConditionLibrary.h"

void UModioCommonNotificationController::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUINotificationController::Register<UModioCommonNotificationController>();
}

void UModioCommonNotificationController::OnNotificationExpired_Implementation(UWidget* Notification)
{
	if (NotificationList)
	{
		NotificationList->RemoveChild(Notification);
	}
	MappedManualParams.Remove(TStrongObjectPtr<UWidget>(Notification));
	MappedParams.Remove(TStrongObjectPtr<UWidget>(Notification));
}

void UModioCommonNotificationController::NativeDisplayNotification(const TScriptInterface<IModioUINotification>& Notification)
{
	IModioUINotificationController::NativeDisplayNotification(Notification);
	if (NotificationList)
	{
		UWidget* UnderlyingWidget = IModioUINotification::Execute_GetAsWidget(Notification.GetObject());
		UE_LOG(ModioUI, Log, TEXT("Displaying notification %s"), *UnderlyingWidget->GetName());
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
	UE_LOG(ModioUI, Log, TEXT("Displaying notification with error code '%s'. Value: %d"), *Params.ErrorCode.GetErrorMessage(), static_cast<int32>(Params.ErrorCode.GetValue()));

	// Check if the same notification is already displayed and if so, move it to the front of the list and avoid creating a new one
	if (RefreshNotificationOrder(Params))
	{
		return;
	}

	if (ErrorNotificationClass && SuccessNotificationClass)
	{
		TSubclassOf<UWidget> ActualClass;
		TSubclassOf<UUserWidget> NotificationWidgetClass = [this, &Params]() {
			bool bInstallOrUpdateCancelled = UModioErrorConditionLibrary::ErrorCodeMatches(Params.ErrorCode, EModioErrorCondition::InstallOrUpdateCancelled);
			if (!Params.ErrorCode || bInstallOrUpdateCancelled)
			{
				if (bInstallOrUpdateCancelled)
				{
					UE_LOG(ModioUI, Log, TEXT("Showing InstallOrUpdateCancelled notification as success"));
				}
				return SuccessNotificationClass;
			}
			return ErrorNotificationClass;
		}();
		if (UBlueprintGeneratedClass* GeneratedClass = Cast<UBlueprintGeneratedClass>(NotificationWidgetClass.Get()))
		{
			if (GeneratedClass->ImplementsInterface(UModioUINotification::StaticClass()))
			{
				ActualClass = GeneratedClass;
			}
		}
		else if (ErrorNotificationClass->ImplementsInterface(UModioUINotification::StaticClass()) && SuccessNotificationClass->ImplementsInterface(UModioUINotification::StaticClass()))
		{
			ActualClass = NotificationWidgetClass.Get();
		}
		if (*ActualClass)
		{
			UWidget* NotificationWidget = UModioCommonNotificationWidgetBase::CreateFromParams(ActualClass, Params, this);
			MappedParams.Add(Params, TStrongObjectPtr<UWidget>(NotificationWidget));
			Execute_DisplayNotification(this, NotificationWidget);
		}
	}
}

void UModioCommonNotificationController::NativeDisplayNotificationManual(const FText& Title, const FText& Message, bool bIsError)
{
	IModioUINotificationController::NativeDisplayNotificationManual(Title, Message, bIsError);
	UE_LOG(ModioUI, Log, TEXT("Displaying manual notification %s with title %s (error: %d)"), *Message.ToString(), *Title.ToString(), bIsError);

	// Check if the same notification is already displayed and if so, move it to the front of the list and avoid creating a new one
	if (RefreshNotificationOrder({Title, Message, bIsError}))
	{
		return;
	}

	if (NotificationList && ErrorNotificationClass && SuccessNotificationClass && GetWorld() && !GetWorld()->bIsTearingDown)
	{
		UUserWidget* UnderlyingWidget = CreateWidget<UUserWidget>(this, bIsError ? ErrorNotificationClass : SuccessNotificationClass);
		if (!UnderlyingWidget)
		{
			return;
		}
		IModioUINotification::Execute_ConfigureManual(UnderlyingWidget, Title, Message, bIsError);
		MappedManualParams.Add({Title, Message, bIsError}, TStrongObjectPtr<UWidget>(UnderlyingWidget));
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

bool UModioCommonNotificationController::RefreshNotificationOrder(const FModioManualNotificationParams& Params)
{
	if (const TStrongObjectPtr<UWidget>* Notification = MappedManualParams.Find(Params))
	{
		MoveNotificationToFront(Notification->Get());
		return true;
	}
	return false;
}

bool UModioCommonNotificationController::RefreshNotificationOrder(const FModioNotificationParams& Params)
{
	if (const TStrongObjectPtr<UWidget>* Notification = MappedParams.Find(Params))
	{
		MoveNotificationToFront(Notification->Get());
		return true;
	}
	return false;
}

void UModioCommonNotificationController::MoveNotificationToFront(UWidget* Notification)
{
	if (NotificationList && Notification && NotificationList->HasChild(Notification))
	{
		NotificationList->RemoveChild(Notification);
		NotificationList->AddChild(Notification);
		IModioUINotification::Execute_Display(Notification);
	}
}
