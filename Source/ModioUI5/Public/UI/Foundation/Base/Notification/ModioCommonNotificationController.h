/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UI/Interfaces/IModioUINotificationController.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "ModioCommonNotificationController.generated.h"

class UPanelWidget;

/**
 * @brief Notification Controller that displays notifications in a list
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonNotificationController
	: public UModioCommonActivatableWidget,
	  public IModioUINotificationController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI")
	int32 MaxNotificationsOnScreen = 5;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	virtual void OnNotificationExpired(UWidget* Notification);

	virtual void NativeDisplayNotification(const TScriptInterface<class IModioUINotification>& Notification) override;
	virtual void NativeDisplayNotificationParams(const FModioNotificationParams& Params) override;
	virtual void NativeDisplayNotificationManual(const FText& Title, const FText& Message, bool bIsError) override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "ModioUINotification"), Category = "Mod.io Common UI")
	TSubclassOf<UUserWidget> ErrorNotificationClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "ModioUINotification"), Category = "Mod.io Common UI")
	TSubclassOf<UUserWidget> SuccessNotificationClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UPanelWidget> NotificationList;
};
