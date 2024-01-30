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
#include "Types/ModioUnsigned64.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/Foundation/Utilities/ModioCommonTickableWidget.h"
#include "ModioCommonStorageSpaceTrackerWidget.generated.h"

/**
 * This widget is used to track the storage space
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonStorageSpaceTrackerWidget : public UTickableModioCommonWidget
{
	GENERATED_BODY()

public:
	UModioCommonStorageSpaceTrackerWidget();
	
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnStorageSpaceTrackerUpdatedFast, FModioUnsigned64 /*UsedSpace*/, FModioUnsigned64 /*FreeSpace*/, FModioUnsigned64 /*TotalSpace*/);
	/**
	 * Called when the storage space tracker is updated
	 */
	FOnStorageSpaceTrackerUpdatedFast OnStorageSpaceTrackerUpdatedFast;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStorageSpaceTrackerUpdated, FModioUnsigned64, UsedSpace, FModioUnsigned64, FreeSpace, FModioUnsigned64, TotalSpace);
	/**
	 * Called when the storage space tracker is updated. Suitable for use in Blueprints
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio UI")
	FOnStorageSpaceTrackerUpdated OnStorageSpaceTrackerUpdated;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Mod.io Common UI|Preview")
	int64 PreviewTotalSpace = 17179869184;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0), Category = "Mod.io Common UI|Preview")
	int64 PreviewFreeSpace = 2147483648;
#endif

protected:
	//~ Begin UTickableModioCommonWidget Interface
	virtual void Tick(float DeltaTime) override;
	//~ End UTickableModioCommonWidget Interface
public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface
protected:

	/**
	 * Updates the storage space tracker
	 * @param FreeSpace The amount of free space
	 * @param TotalSpace The total amount of space
	 */
	virtual void Update(uint64 FreeSpace, uint64 TotalSpace);
};