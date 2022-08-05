// Fill out your copyright notice in the Description page of Project Settings.

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
 *
 */
UCLASS(Abstract, BlueprintType)
class MODIOUI_API UModioNotificationWidgetBase : public UModioUserWidgetBase, public IModioUINotification
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioNotificationStyle"), Category="Widgets")
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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category="Widgets")
	bool bPreviewAsError;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category="Widgets")
	FModioNotificationParams PreviewParameters;
#endif

#if WITH_EDITOR
	virtual void SynchronizeProperties() override;
#endif
public:
	UFUNCTION(BlueprintCallable, Category="ModioNotificationWidgetBase")
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

UCLASS()
class MODIOUI_API UModioNotificationErrorWidgetBase : public UModioNotificationWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeConfigure(const FModioNotificationParams& Params);
	FDateTime StartTime;
	bool bDisplayStarted = false;
	virtual void NativeOnDisplay();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioImage* NotificationBackground;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioImage* StatusIndicator;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* NotificationTitle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidgetOptional))
	UModioRichTextBlock* NotificationMessage;

public:
};