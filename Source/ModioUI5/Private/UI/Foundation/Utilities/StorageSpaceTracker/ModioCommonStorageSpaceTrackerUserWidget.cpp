/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Utilities/StorageSpaceTracker/ModioCommonStorageSpaceTrackerUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Libraries/ModioSDKLibrary.h"
#include "UI/Foundation/Components/Image/ModioCommonImage.h"
#include "UI/Foundation/Components/ProgressBar/ModioCommonProgressBar.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Utilities/StorageSpaceTracker/ModioCommonStorageSpaceTrackerWidget.h"
#include "UI/Foundation/Utilities/StorageSpaceTracker/ModioCommonStorageSpaceTrackerUserWidgetStyle.h"
#include "UI/Settings/Params/ModioCommonStorageSpaceTrackerParams.h"

void UModioCommonStorageSpaceTrackerUserWidget::SetStyle(TSubclassOf<UModioCommonStorageSpaceTrackerUserWidgetStyle> InStyle)
{
	ModioStyle = InStyle;
	SynchronizeProperties();
}

bool UModioCommonStorageSpaceTrackerUserWidget::Initialize()
{
	const bool bSuperInitialized = Super::Initialize();
	if (Tracker)
	{
		Tracker->OnStorageSpaceTrackerUpdatedFast.RemoveAll(this);
		Tracker->OnStorageSpaceTrackerUpdatedFast.AddUObject(this, &UModioCommonStorageSpaceTrackerUserWidget::OnStorageSpaceTrackerUpdated);
	}
	return bSuperInitialized;
}

void UModioCommonStorageSpaceTrackerUserWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonStorageSpaceTrackerParamsSettings* Settings = GetDefault<UModioCommonStorageSpaceTrackerParamsSettings>())
	{
		if (UsedSpaceLabelTextBlock)
		{
			UsedSpaceLabelTextBlock->SetText(Settings->UsedSpaceLabelText);
		}

		if (FreeSpaceLabelTextBlock)
		{
			FreeSpaceLabelTextBlock->SetText(Settings->FreeSpaceLabelText);
		}

		if (TotalSpaceLabelTextBlock)
		{
			TotalSpaceLabelTextBlock->SetText(Settings->TotalSpaceLabelText);
		}
	}

	if (UModioCommonStorageSpaceTrackerUserWidgetStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (IconImage)
		{
			IconImage->SetStyle(StyleCDO->IconImageStyle);
		}
		
		if (UsedSpaceLabelTextBlock)
		{
			UsedSpaceLabelTextBlock->SetStyle(StyleCDO->UsedSpaceLabelTextStyle);
		}

		if (UsedSpaceTextBlock)
		{
			UsedSpaceTextBlock->SetStyle(StyleCDO->UsedSpaceTextStyle);
		}

		if (FreeSpaceLabelTextBlock)
		{
			FreeSpaceLabelTextBlock->SetStyle(StyleCDO->FreeSpaceLabelTextStyle);
		}

		if (FreeSpaceTextBlock)
		{
			FreeSpaceTextBlock->SetStyle(StyleCDO->FreeSpaceTextStyle);
		}

		if (TotalSpaceLabelTextBlock)
		{
			TotalSpaceLabelTextBlock->SetStyle(StyleCDO->TotalSpaceLabelTextStyle);
		}

		if (TotalSpaceTextBlock)
		{
			TotalSpaceTextBlock->SetStyle(StyleCDO->TotalSpaceTextStyle);
		}

		if (StorageSpaceProgressBar)
		{
			StorageSpaceProgressBar->SetStyle(StyleCDO->StorageSpaceProgressBarStyle);
		}
	}
}

void UModioCommonStorageSpaceTrackerUserWidget::OnStorageSpaceTrackerUpdated(FModioUnsigned64 UsedSpace, FModioUnsigned64 FreeSpace, FModioUnsigned64 TotalSpace)
{
	if (UsedSpaceTextBlock)
	{
		const FText UsedSpaceText = UModioSDKLibrary::Filesize_ToString(UsedSpace.Underlying, MinDecimals, MaxDecimals);
		UsedSpaceTextBlock->SetText(UsedSpaceText);
	}

	if (FreeSpaceTextBlock)
	{
		const FText FreeSpaceText = UModioSDKLibrary::Filesize_ToString(FreeSpace.Underlying, MinDecimals, MaxDecimals);
		FreeSpaceTextBlock->SetText(FreeSpaceText);
	}

	if (TotalSpaceTextBlock)
	{
		const FText TotalSpaceText = UModioSDKLibrary::Filesize_ToString(TotalSpace.Underlying, MinDecimals, MaxDecimals);
		TotalSpaceTextBlock->SetText(TotalSpaceText);
	}

	if (StorageSpaceProgressBar)
	{
		StorageSpaceProgressBar->SetPercent(UsedSpace.Underlying / static_cast<float>(TotalSpace.Underlying));
	}
}
