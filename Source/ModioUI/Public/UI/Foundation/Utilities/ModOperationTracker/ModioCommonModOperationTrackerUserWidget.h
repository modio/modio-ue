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
#include "Types/ModioModCollectionEntry.h"
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Interfaces/IModioExtendedModInfoUIDetails.h"
#include "UI/Interfaces/IModioUIModEnableWidget.h"
#include "ModioCommonModOperationTrackerUserWidget.generated.h"

class UModioCommonModOperationTrackerUserWidgetStyle;
class UModioCommonTextBlock;
class UModioCommonImage;
class UModioCommonProgressBar;
class UModioCommonModOperationTrackerWidget;

/**
 * This widget is used to display the progress of a mod operation
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModOperationTrackerUserWidget
	: public UModioCommonActivatableWidget,
	  public IModioModInfoUIDetails,
	  public IModioExtendedModInfoUIDetails,
	  public IModioUIModEnableWidget,
	  public IModioUIModManagementEventReceiver,
	  public IModioUISubscriptionsChangedReceiver,
	  public IModioUIUserChangedReceiver
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Mod Operation Tracker within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonModOperationTrackerUserWidgetStyle> InStyle);

protected:
	/** The style of the Mod Operation Tracker within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonModOperationTrackerUserWidgetStyle> ModioStyle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonModOperationTrackerWidget> Tracker;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonImage> IconImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> ModTitleTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> OverallOperationPercentageLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> OverallOperationPercentageTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> QueuedOperationNumberLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> QueuedOperationNumberTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> SpeedLabelTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonTextBlock> SpeedTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonProgressBar> ModOperationProgressBar;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MinDecimalsProgress = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MaxDecimalsProgress = 0;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MinDecimalsSpeed = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Properties")
	int32 MaxDecimalsSpeed = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	bool bTrackAnyMods;

public:
	/**
	 * Sets the mod to track
	 * @param ModID The ID of the mod to track
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod Operation Tracker")
	void SetTrackingModID(FModioModID ModID);

	/**
	 * Gets the number of queued mods
	 * @return The number of queued mods
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod Operation Tracker")
	int32 GetNumOfQueuedMods() const;

	/**
	 * Gets the current mod being downloaded and extracted
	 * @return Whether we have a current mod being downloaded
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod Operation Tracker")
	bool GetCurrentDownloadingMod(FModioModCollectionEntry& OutDownloadingMod);

protected:
	//~ Begin IModioUIUserChangedReceiver Interface
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	//~ End IModioUIUserChangedReceiver Interface

	//~ Begin IModioUIModManagementEventReceiver Interface
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	//~ End IModioUIModManagementEventReceiver Interface

	//~ Begin IModioModInfoUIDetails Interface
	virtual FModioModID GetModID_Implementation() override;
	virtual FModioModInfo GetFullModInfo_Implementation() override;
	//~ End IModioModInfoUIDetails Interface

	//~ Begin IModioExtendedModInfoUIDetails Interface
	virtual bool IsModSubscribed_Implementation() const override;
	virtual bool IsModDownloading_Implementation() const override;
	virtual bool IsModExtracting_Implementation() const override;
	virtual bool IsModInQueue_Implementation() const override;
	virtual bool IsModEnabled_Implementation() override;
	virtual bool IsModInstalled_Implementation() const override;
	virtual bool HasModErrors_Implementation() const override;
	//~ End IModioExtendedModInfoUIDetails Interface

protected:
	//~ Begin UWidget Interface
	virtual bool Initialize() override;
	virtual void NativeOnInitialized() override;
public:
	virtual void SynchronizeProperties() override;
protected:
	//~ End UWidget Interface

	/**
	 * Called when the progress of the mod operation has been updated
	 * @param Current The current progress in bytes
	 * @param Total The total progress in bytes
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Mod Operation Tracker")
	void OnModOperationTrackerProgressUpdated(FModioUnsigned64 Current, FModioUnsigned64 Total);

	/**
	 * Called when the speed of the mod operation has been updated
	 * @param DeltaBytes The delta bytes
	 * @param DeltaTime The delta time
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Mod Operation Tracker")
	void OnModOperationTrackerSpeedUpdated(FModioUnsigned64 DeltaBytes, double DeltaTime);

	/**
	 * Called when the overall progress of the mod operation has been updated
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Mod Operation Tracker")
	void UpdateQueuedOperationNumber();
};
