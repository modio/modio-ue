/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Foundation/Utilities/StorageSpaceTracker/ModioCommonStorageSpaceTrackerWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Widgets/Layout/SSpacer.h"

UModioCommonStorageSpaceTrackerWidget::UModioCommonStorageSpaceTrackerWidget()
{
	UWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UModioCommonStorageSpaceTrackerWidget::Tick(float DeltaTime)
{
	if (OnStorageSpaceTrackerUpdatedFast.IsBound() || OnStorageSpaceTrackerUpdated.IsBound())
	{
		uint64 TotalBytes;
		uint64 FreeBytes;
		if (FPlatformMisc::GetDiskTotalAndFreeSpace(FGenericPlatformMisc::ProjectDir(), TotalBytes, FreeBytes))
		{
			Update(FreeBytes, TotalBytes);
		}
	}
}

void UModioCommonStorageSpaceTrackerWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
#if WITH_EDITOR
	if (IsDesignTime())
	{
		Update(PreviewFreeSpace, PreviewTotalSpace);
	}
#endif
}

void UModioCommonStorageSpaceTrackerWidget::Update(uint64 FreeSpace, uint64 TotalSpace)
{
	const uint64 UsedSpace = TotalSpace - FreeSpace;
	if (OnStorageSpaceTrackerUpdatedFast.IsBound())
	{
		OnStorageSpaceTrackerUpdatedFast.Broadcast(FModioUnsigned64(UsedSpace), FModioUnsigned64(FreeSpace), FModioUnsigned64(TotalSpace));
	}
	if (OnStorageSpaceTrackerUpdated.IsBound())
	{
		OnStorageSpaceTrackerUpdated.Broadcast(FModioUnsigned64(UsedSpace), FModioUnsigned64(FreeSpace), FModioUnsigned64(TotalSpace));
	}
}
