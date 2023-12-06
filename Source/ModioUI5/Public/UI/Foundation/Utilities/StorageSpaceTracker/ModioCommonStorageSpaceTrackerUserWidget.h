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
#include "Types/ModioUnsigned64.h"
#include "ModioCommonStorageSpaceTrackerUserWidget.generated.h"

class UModioCommonProgressBar;
class UModioCommonTextBlock;
class UModioCommonStorageSpaceTrackerUserWidgetStyle;
class UModioCommonStorageSpaceTrackerWidget;
class UModioCommonImage;
class UModioCommonQuickAccessTabViewStyle;

/**
 * Displays the amount of storage space used and free
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonStorageSpaceTrackerUserWidget : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Storage Space Tracker within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonStorageSpaceTrackerUserWidgetStyle> InStyle);

protected:
	/** The style of the Storage Space Tracker within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonStorageSpaceTrackerUserWidgetStyle> ModioStyle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonStorageSpaceTrackerWidget> Tracker;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonImage> IconImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> UsedSpaceLabelTextBlock;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> UsedSpaceTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> FreeSpaceLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> FreeSpaceTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> TotalSpaceLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> TotalSpaceTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonProgressBar> StorageSpaceProgressBar;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MinDecimals = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MaxDecimals = 0;

protected:
	//~ Begin UWidget Interface
	virtual bool Initialize() override;
public:
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Storage Space Tracker")
	void OnStorageSpaceTrackerUpdated(FModioUnsigned64 UsedSpace, FModioUnsigned64 FreeSpace, FModioUnsigned64 TotalSpace);
};
