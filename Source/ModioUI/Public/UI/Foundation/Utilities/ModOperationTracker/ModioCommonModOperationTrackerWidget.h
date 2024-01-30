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
#include "Types/ModioModCollectionEntry.h"
#include "Types/ModioModProgressInfo.h"
#include "Types/ModioUnsigned64.h"
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/Foundation/Utilities/ModioCommonTickableWidget.h"
#include "UI/Interfaces/IModioExtendedModInfoUIDetails.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUIModEnableWidget.h"
#include "UObject/Object.h"
#include "ModioCommonModOperationTrackerWidget.generated.h"

/**
 * This widget is used to track the progress of a mod operation, such as downloading, installing, or enabling a mod
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModOperationTrackerWidget
	: public UTickableModioCommonWidget,
	  public IModioModInfoUIDetails,
	  public IModioExtendedModInfoUIDetails,
	  public IModioUIModManagementEventReceiver,
	  public IModioUIModEnableWidget
{
	GENERATED_BODY()

public:
	UModioCommonModOperationTrackerWidget();
	
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	void SetTrackingModID(FModioModID ModID);

	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	void SetTrackAnyMods(bool bInTrackAnyMods);

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
	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCommonModOperationTrackerProgressFast, FModioUnsigned64 /*Current*/, FModioUnsigned64 /*Total*/);
	/**
	 * Fired when the progress of the mod operation tracker is updated
	 */
	FOnCommonModOperationTrackerProgressFast OnProgressFast;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCommonModOperationTrackerProgress, FModioUnsigned64, Current, FModioUnsigned64, Total);
	/**
	 * Fired when the progress of the mod operation tracker is updated. Suitable for use in Blueprints
	 */
	UPROPERTY(BlueprintAssignable, Category = "Mod.io Common UI")
	FOnCommonModOperationTrackerProgress OnProgress;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCommonModOperationTrackerSpeedFast, FModioUnsigned64 /*DeltaBytes*/, double /*DeltaTime*/);
	/**
	 * Fired when the speed of the mod operation tracker is updated
	 */
	FOnCommonModOperationTrackerSpeedFast OnSpeedFast;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCommonModOperationTrackerSpeed, FModioUnsigned64, DeltaBytes, double, DeltaTime);
	/**
	 * Fired when the speed of the mod operation tracker is updated. Suitable for use in Blueprints
	 */
	UPROPERTY(BlueprintAssignable, Category = "Mod.io Common UI")
	FOnCommonModOperationTrackerSpeed OnSpeed;

public:
#if WITH_EDITORONLY_DATA
	/**
	 * The total progress of the mod operation tracker, in bytes (editor only)
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0, ClampMin = 0), Category = "Mod.io Common UI|Preview")
	int64 PreviewTotal = 4096;

	/**
	 * The current progress of the mod operation tracker, in bytes (editor only)
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0, ClampMin = 0), Category = "Mod.io Common UI|Preview")
	int64 PreviewCurrent = 1024;

	/**
	 * The progress state of the mod operation tracker (editor only)
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	EModioModProgressState PreviewProgressState = EModioModProgressState::Downloading;

	/**
	 * The mod state of the mod operation tracker (editor only)
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	EModioModState PreviewModState = EModioModState::Downloading;

	/**
	 * Whether the mod is enabled (editor only)
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	bool bPreviewIsModEnabled = false;

	/**
	 * Whether the mod is installed (editor only)
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mod.io Common UI|Preview")
	bool bPreviewHasModErrors = false;
#endif

public:
	/**
	 * Gets the total progress of the mod operation tracker
	 * @return The total progress
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	int64 GetPreviewTotal() const;

	/**
	 * Gets the current progress of the mod operation tracker
	 * @return The current progress
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	int64 GetPreviewCurrent() const;

protected:
	//~ Begin UWidget Interface
	virtual void OnWidgetRebuilt() override;
public:
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

protected:
	//~ Begin UModioCommonModOperationTrackerWidget Interface
	virtual void Tick(float DeltaTime) override;
	//~ End UModioCommonModOperationTrackerWidget Interface
	
protected:
	/**
	 * Updates the progress of the mod operation tracker
	 * @param ProgressInfo The progress info to update with
	 */
	virtual void UpdateProgress(const FModioModProgressInfo& ProgressInfo);

	/**
	 * Hide the progress
	 */
	virtual void HideProgress();

	/**
	 * Broadcasts the progress
	 * @param Current The current progress
	 * @param Total The total progress
	 */
	virtual void BroadcastProgress(const FModioUnsigned64& Current, const FModioUnsigned64& Total);

	/**
	 * Broadcasts the speed
	 * @param DeltaBytes The delta bytes
	 * @param DeltaTime The delta time
	 */
	virtual void BroadcastSpeed(const FModioUnsigned64& DeltaBytes, double DeltaTime);

	/** Previously broadcasted progress value */
	FModioUnsigned64 PreviousProgressValue = FModioUnsigned64(0);

	/** The previous update time */
	FDateTime PreviousUpdateTime;

	//~ Begin IModioUIModManagementEventReceiver Interface
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	//~ End IModioUIModManagementEventReceiver Interface

private:
	/** The mod we are tracking */
	TOptional<FModioModID> TrackingModID;

	/** Whether we are tracking any mods */
	bool bTrackAnyMods = false;

	/** Whether there are any errors */
	bool bHasErrors = false;
};
