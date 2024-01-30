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
#include "UI/Foundation/Base/Notification/ModioCommonNotificationWidgetBase.h"
#include "Misc/DateTime.h"
#include "ModioCommonNotificationWidget.generated.h"

class UModioCommonRichTextBlock;
class UModioCommonBorder;
class UModioCommonImage;
class UModioCommonNotificationWidgetStyle;

/**
 * Notification Widget that displays a notification message
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonNotificationWidget : public UModioCommonNotificationWidgetBase
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Notification Widget within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonNotificationWidgetStyle> InStyle);

protected:
	/** The style of the notification widget within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonNotificationWidgetStyle> ModioStyle;

	//~ Begin UUserWidget Interface
public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~ End UUserWidget Interface

	//~ Begin IModioUINotification Interface
	virtual void NativeConfigure(const FModioNotificationParams& Params) override;
	virtual void NativeConfigureManual(const FText& Title, const FText& Message, bool bIsError) override;
	virtual void NativeOnDisplay() override;
	//~ End IModioUINotification Interface

	/** The background border of the notification */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonBorder> BackgroundBorder;

	/** The status border of the notification status */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonBorder> StatusBackgroundBorder;

	/** The image that displays the notification status */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonImage> StatusIndicatorImage;

	/** The text block that displays the notification title */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI", meta = (BindWidget))
	TObjectPtr<UModioCommonRichTextBlock> NotificationTitleTextBlock;

	/** The text block that displays the notification message */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonRichTextBlock> NotificationMessageTextBlock;

	/** The time at which the notification was displayed */
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Mod.io Common UI|Properties")
	FDateTime StartTime;

	/** Whether the notification has already been displayed */
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Mod.io Common UI|Properties")
	bool bDisplayStarted = false;
};
