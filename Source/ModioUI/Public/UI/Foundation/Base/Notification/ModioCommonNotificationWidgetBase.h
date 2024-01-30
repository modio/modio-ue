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
class MODIOUI_API UModioCommonNotificationWidgetBase
	: public UModioCommonActivatableWidget,
	  public IModioUINotification
{
	GENERATED_BODY()

protected:
	//~ Begin IModioUINotification Interface
	virtual UWidget* NativeGetAsWidget() override;
	//~ End IModioUINotification Interface

	/** Expire delegate to call when the notification expires */
	UPROPERTY(BlueprintReadWrite, Category = "Mod.io Common UI")
	FOnNotificationExpired ExpireDelegate;

	/** Expiry duration of the notification, in seconds */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI")
	float ExpiryDuration = 0;

	//~ Begin IModioUINotification Interface
	virtual void NativeSetNotificationExpireHandler(const FOnNotificationExpired& InDelegate) override;
	virtual void NativeConfigure(const FModioNotificationParams& Params) override;
	virtual void NativeConfigureManual(const FText& Title, const FText& Message, bool bIsError) override;
	//~ End IModioUINotification Interface
public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface
protected:

#if WITH_EDITORONLY_DATA
	/** True if the widget should preview as an error (editor only) */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "Mod.io Common UI|Editor Preview")
	bool bPreviewAsError;

	/** Preview parameters to use when previewing the notification (editor only) */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "Mod.io Common UI|Editor Preview")
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
