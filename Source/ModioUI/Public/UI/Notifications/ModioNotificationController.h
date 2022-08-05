// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioOverlay.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Interfaces/IModioUINotification.h"
#include "UI/Interfaces/IModioUINotificationController.h"
#include "UObject/UObjectGlobals.h"

#include "ModioNotificationController.generated.h"

/**
 *
 */
UCLASS(Abstract, BlueprintType)
class MODIOUI_API UModioNotificationControllerBase : public UModioUserWidgetBase, public IModioUINotificationController
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	virtual void OnNotificationExpired(UWidget* Notification);

	virtual void NativeDisplayNotification(const TScriptInterface<class IModioUINotification>& Notification) override;
	virtual void NativeDisplayErrorNotification(const FModioNotificationParams& Params) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "ModioUINotification"), Category="Widgets")
	TSubclassOf<UWidget> ErrorNotificationClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	class UVerticalBox* NotificationList;
};
