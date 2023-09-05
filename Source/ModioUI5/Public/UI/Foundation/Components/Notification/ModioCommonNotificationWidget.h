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
 * @brief Notification Widget that displays a notification message
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonNotificationWidget : public UModioCommonNotificationWidgetBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the style of the Notification Widget within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonNotificationWidgetStyle> InStyle);

protected:
	/** @brief The style of the notification widget within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonNotificationWidgetStyle> ModioStyle;

public:
	virtual void SynchronizeProperties() override;
protected:

	virtual void NativeConfigure(const FModioNotificationParams& Params) override;
	virtual void NativeConfigureManual(const FText& Title, const FText& Message, bool bIsError) override;
	virtual void NativeOnDisplay() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonBorder> BackgroundBorder;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonBorder> StatusBackgroundBorder;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonImage> StatusIndicatorImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UModioCommonRichTextBlock> NotificationTitleTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonRichTextBlock> NotificationMessageTextBlock;

	FDateTime StartTime;
	bool bDisplayStarted = false;
};
