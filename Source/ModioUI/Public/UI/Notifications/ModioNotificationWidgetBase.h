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

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Misc/DateTime.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Interfaces/IModioUINotification.h"
#include "UI/Styles/ModioUIColorRef.h"

#include "ModioNotificationWidgetBase.generated.h"

/**
* Base class definition for a notification widget with an expiry duration and preview parameters
**/
UCLASS(Abstract, BlueprintType)
class MODIOUI_API UModioNotificationWidgetBase : public UModioUserWidgetBase, public IModioUINotification
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioNotificationStyle"), Category = "Widgets")
	FModioUIStyleRef NotificationStyle /* = FModioUIStyleRef {"DefaultErrorNotificationStyle"}*/;
	virtual UWidget* NativeGetAsWidget() override
	{
		return Cast<UWidget>(this);
	}

	FOnNotificationExpired ExpireDelegate;
	virtual void NativeSetNotificationExpireHandler(const FOnNotificationExpired& InDelegate);
	float ExpiryDuration = 0;
	virtual void NativeConfigure(const FModioNotificationParams& Params) override;

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "Widgets")
	bool bPreviewAsError;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "Widgets")
	FModioNotificationParams PreviewParameters;
#endif

#if WITH_EDITOR
	virtual void SynchronizeProperties() override;
#endif
public:
	/**
	* Create a notification widget based on a subclass, params, and a reference
	* @param NotificationClass The widget subclass the new instance will be part of
	* @param Params The parameters the new instance should store
	* @param Outer The reference widget the new instance should be linked to
	* @return An instance of a Widget subclass that is a notification
	**/
	UFUNCTION(BlueprintCallable, Category = "ModioNotificationWidgetBase")
	static UWidget* CreateFromParams(TSubclassOf<UWidget> NotificationClass, const FModioNotificationParams& Params,
									 UWidget* Outer)
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

/**
* Base class definition for a notification error
**/
UCLASS()
class MODIOUI_API UModioNotificationErrorWidgetBase : public UModioNotificationWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeConfigure(const FModioNotificationParams& Params) override;
	virtual void NativeConfigureManual(const FText& Title, const FText& Message, bool bIsError) override;
	virtual void NativeOnDisplay() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioImage* StatusColorBackground;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioImage* StatusIndicator;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidgetOptional))
	UModioRichTextBlock* NotificationTitle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidgetOptional))
	UModioRichTextBlock* NotificationMessage;

	FDateTime StartTime;
	bool bDisplayStarted = false;

public:
};