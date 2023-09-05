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
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/Interfaces/IModioUINotification.h"
#include "ModioCommonNotificationWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonNotificationWidgetBase
	: public UModioCommonActivatableWidget,
	  public IModioUINotification
{
	GENERATED_BODY()

protected:

	virtual UWidget* NativeGetAsWidget() override;
	
	virtual void NativeSetNotificationExpireHandler(const FOnNotificationExpired& InDelegate) override;
	FOnNotificationExpired ExpireDelegate;
	float ExpiryDuration = 0;

	virtual void NativeConfigure(const FModioNotificationParams& Params) override;
	virtual void NativeConfigureManual(const FText& Title, const FText& Message, bool bIsError) override;
public:
	virtual void SynchronizeProperties() override;
protected:

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "Widgets")
	bool bPreviewAsError;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "Widgets")
	FModioNotificationParams PreviewParameters;
#endif

public:
	/**
	 * Create a notification widget based on a subclass, params, and a reference
	 *
	 * @param NotificationClass The widget subclass the new instance will be part of
	 * @param Params The parameters the new instance should store
	 * @param Outer The reference widget the new instance should be linked to
	 * @return An instance of a Widget subclass that is a notification
	 */
	UFUNCTION(BlueprintCallable, Category = "ModioNotificationWidgetBase")
	static UWidget* CreateFromParams(TSubclassOf<UWidget> NotificationClass, const FModioNotificationParams& Params, UWidget* Outer)
	{
		UWidget* NotificationInstance = nullptr;
		if (NotificationClass->IsChildOf<UUserWidget>())
		{
			NotificationInstance = Cast<UWidget>(CreateWidget(Outer, *NotificationClass));
		}
		else
		{
			NotificationInstance = NewObject<UWidget>(Outer, *NotificationClass);
		}

		if (NotificationInstance)
		{
			if (NotificationInstance->Implements<UModioUINotification>())
			{
				IModioUINotification::Execute_Configure(NotificationInstance, Params);
				return NotificationInstance;
			}
		}
		return nullptr;
	}
};
