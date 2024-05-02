/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerUserWidget.h"

#include "ModioSubsystem.h"
#include "Algo/Count.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Types/ModioModCollectionEntry.h"
#include "UI/Foundation/Components/Image/ModioCommonImage.h"
#include "UI/Foundation/Components/ProgressBar/ModioCommonProgressBar.h"
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlock.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerUserWidgetStyle.h"
#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerWidget.h"
#include "UI/Settings/ModioCommonUISettings.h"
#include "UI/Settings/Params/ModioCommonModOperationTrackerParams.h"

void UModioCommonModOperationTrackerUserWidget::SetStyle(TSubclassOf<UModioCommonModOperationTrackerUserWidgetStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonModOperationTrackerUserWidget::SetTrackingModID(FModioModID ModID)
{
	if (Tracker)
	{
		Tracker->SetTrackingModID(ModID);
		SynchronizeProperties();
	}
}

int32 UModioCommonModOperationTrackerUserWidget::GetNumOfQueuedMods() const
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return 0;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& UserSubscriptions = Subsystem->QueryUserSubscriptions();

	return Algo::CountIf(UserSubscriptions, [](const TPair<FModioModID, FModioModCollectionEntry>& Entry) {
		const FModioModCollectionEntry& ModEntry = Entry.Value;
		return ModEntry.GetModState() == EModioModState::UpdatePending
		|| ModEntry.GetModState() == EModioModState::Downloading
		|| ModEntry.GetModState() == EModioModState::Extracting
		|| ModEntry.GetModState() == EModioModState::InstallationPending;
	});
}

bool UModioCommonModOperationTrackerUserWidget::GetCurrentDownloadingMod(FModioModCollectionEntry& OutDownloadingMod) 
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		const TMap<FModioModID, FModioModCollectionEntry>& UserSubscriptions = Subsystem->QueryUserSubscriptions();

		if (UserSubscriptions.Num() > 0)
		{
			TArray<FModioModCollectionEntry> DownloadingEntries;
			UserSubscriptions.GenerateValueArray(DownloadingEntries);

			DownloadingEntries.RemoveAll([](const FModioModCollectionEntry& Entry) 
			{
				return Entry.GetModState() != EModioModState::Downloading && Entry.GetModState() != EModioModState::Extracting;
			});

			if (DownloadingEntries.Num() > 0) 
			{
				OutDownloadingMod = DownloadingEntries[0];
				return true;
			}
		}
	}
	return false;
}

void UModioCommonModOperationTrackerUserWidget::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	UpdateQueuedOperationNumber();
	SynchronizeProperties();
}

void UModioCommonModOperationTrackerUserWidget::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);
	if (bTrackAnyMods || (Execute_GetModID(this) == Event.ID))
	{
		UpdateQueuedOperationNumber();
	}
}

FModioModID UModioCommonModOperationTrackerUserWidget::GetModID_Implementation()
{
	if (Tracker && Tracker->Implements<UModioModInfoUIDetails>())
	{
		return Execute_GetModID(Tracker);
	}
	return FModioModID();
}

FModioModInfo UModioCommonModOperationTrackerUserWidget::GetFullModInfo_Implementation()
{
	if (Tracker && Tracker->Implements<UModioModInfoUIDetails>())
	{
		return Execute_GetFullModInfo(Tracker);
	}
	return FModioModInfo();
}

bool UModioCommonModOperationTrackerUserWidget::IsModSubscribed_Implementation() const
{
	if (Tracker && Tracker->Implements<UModioExtendedModInfoUIDetails>())
	{
		return Execute_IsModSubscribed(Tracker);
	}
	return false;
}

bool UModioCommonModOperationTrackerUserWidget::IsModDownloading_Implementation() const
{
	if (Tracker && Tracker->Implements<UModioExtendedModInfoUIDetails>())
	{
		return Execute_IsModDownloading(Tracker);
	}
	return false;
}

bool UModioCommonModOperationTrackerUserWidget::IsModExtracting_Implementation() const
{
	if (Tracker && Tracker->Implements<UModioExtendedModInfoUIDetails>())
	{
		return Execute_IsModExtracting(Tracker);
	}
	return false;
}

bool UModioCommonModOperationTrackerUserWidget::IsModInQueue_Implementation() const
{
	if (Tracker && Tracker->Implements<UModioExtendedModInfoUIDetails>())
	{
		return Execute_IsModInQueue(Tracker);
	}
	return false;
}

bool UModioCommonModOperationTrackerUserWidget::IsModEnabled_Implementation()
{
	if (Tracker && Tracker->Implements<UModioExtendedModInfoUIDetails>())
	{
		return Execute_IsModEnabled(Tracker);
	}
	return false;
}

bool UModioCommonModOperationTrackerUserWidget::IsModInstalled_Implementation() const
{
	if (Tracker && Tracker->Implements<UModioExtendedModInfoUIDetails>())
	{
		return Execute_IsModInstalled(Tracker);
	}
	return false;
}

bool UModioCommonModOperationTrackerUserWidget::HasModErrors_Implementation() const
{
	if (Tracker && Tracker->Implements<UModioExtendedModInfoUIDetails>())
	{
		return Execute_HasModErrors(Tracker);
	}
	return false;
}

bool UModioCommonModOperationTrackerUserWidget::Initialize()
{
	if (Tracker)
	{
		Tracker->OnProgressFast.RemoveAll(this);
		Tracker->OnSpeedFast.RemoveAll(this);
	}
	const bool bSuperInitialized = Super::Initialize();
	if (Tracker)
	{
		Tracker->SetTrackAnyMods(bTrackAnyMods);
		Tracker->OnProgressFast.AddUObject(this, &UModioCommonModOperationTrackerUserWidget::OnModOperationTrackerProgressUpdated);
		Tracker->OnSpeedFast.AddUObject(this, &UModioCommonModOperationTrackerUserWidget::OnModOperationTrackerSpeedUpdated);
		OnModOperationTrackerProgressUpdated(FModioUnsigned64(0), FModioUnsigned64(0));
		OnModOperationTrackerSpeedUpdated(FModioUnsigned64(0), 0);
	}
	return bSuperInitialized;
}

void UModioCommonModOperationTrackerUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUIModEnableWidget::Register<UModioCommonModOperationTrackerUserWidget>();
	IModioUIModManagementEventReceiver::Register<UModioCommonModOperationTrackerUserWidget>();
	IModioUISubscriptionsChangedReceiver::Register<UModioCommonModOperationTrackerUserWidget>();
	IModioUIUserChangedReceiver::Register<UModioCommonModOperationTrackerUserWidget>();
}

void UModioCommonModOperationTrackerUserWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const UModioCommonUISettings* UISettings = GetDefault<UModioCommonUISettings>())
	{
		if (OverallOperationPercentageLabelTextBlock)
		{
			OverallOperationPercentageLabelTextBlock->SetText(UISettings->ModOperationTrackerParams.OverallOperationPercentageLabelText);
		}

		if (QueuedOperationNumberLabelTextBlock)
		{
			QueuedOperationNumberLabelTextBlock->SetText(UISettings->ModOperationTrackerParams.QueuedOperationNumberLabelText);
		}

		if (SpeedLabelTextBlock)
		{
			SpeedLabelTextBlock->SetText(UISettings->ModOperationTrackerParams.SpeedLabelText);
		}
	}

	if (UModioCommonModOperationTrackerUserWidgetStyle* StyleCDO = ModioStyle.GetDefaultObject())
	{
		if (IconImage)
		{
			IconImage->SetStyle(StyleCDO->IconImageStyle);
		}

		if (OverallOperationPercentageLabelTextBlock)
		{
			OverallOperationPercentageLabelTextBlock->SetStyle(StyleCDO->OverallOperationPercentageLabelTextStyle);
		}

		if (OverallOperationPercentageTextBlock)
		{
			OverallOperationPercentageTextBlock->SetStyle(StyleCDO->OverallOperationPercentageTextStyle);
		}

		if (QueuedOperationNumberLabelTextBlock)
		{
			QueuedOperationNumberLabelTextBlock->SetStyle(StyleCDO->QueuedOperationNumberLabelTextStyle);
		}

		if (QueuedOperationNumberTextBlock)
		{
			QueuedOperationNumberTextBlock->SetStyle(StyleCDO->QueuedOperationNumberTextStyle);
		}

		if (SpeedLabelTextBlock)
		{
			SpeedLabelTextBlock->SetStyle(StyleCDO->SpeedLabelTextStyle);
		}

		if (SpeedTextBlock)
		{
			SpeedTextBlock->SetStyle(StyleCDO->SpeedTextStyle);
		}

		if (ModOperationProgressBar)
		{
			ModOperationProgressBar->SetStyle(StyleCDO->ModOperationProgressBarStyle);
		}
	}

	if (ModTitleTextBlock) 
	{
		FModioModCollectionEntry Empty;
		ModTitleTextBlock->SetVisibility(GetCurrentDownloadingMod(Empty) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UModioCommonModOperationTrackerUserWidget::OnModOperationTrackerProgressUpdated_Implementation(FModioUnsigned64 Current, FModioUnsigned64 Total)
{
	const FText CurrentText = UModioSDKLibrary::Filesize_ToString(Current.Underlying, MinDecimalsProgress, MaxDecimalsProgress);
	const FText TotalText = UModioSDKLibrary::Filesize_ToString(Total.Underlying, MinDecimalsProgress, MaxDecimalsProgress);
	const float CurrentModProgress = Total ? (Current / static_cast<float>(Total)) : 0;

	if (OverallOperationPercentageTextBlock)
	{
		const uint8 Percentage = static_cast<uint8>(CurrentModProgress * 100);
		OverallOperationPercentageTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), Percentage)));
	}

	if (ModOperationProgressBar)
	{
		ModOperationProgressBar->SetPercent(CurrentModProgress);
	}

	UpdateQueuedOperationNumber();

	if (ModTitleTextBlock)
	{
		FModioModCollectionEntry DownloadingMod;
		if (GetCurrentDownloadingMod(DownloadingMod)) 
		{
			ModTitleTextBlock->SetText(FText::FromString(DownloadingMod.GetModProfile().ProfileName));
			ModTitleTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else 
		{
			ModTitleTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UModioCommonModOperationTrackerUserWidget::OnModOperationTrackerSpeedUpdated_Implementation(FModioUnsigned64 DeltaBytes, double DeltaTime)
{
	if (DeltaBytes.Underlying > 0)
	{
		const int64 BytesPerSecond = DeltaBytes.Underlying <= 0 ? 0 : DeltaBytes / DeltaTime / 8;
		const FText TotalText = UModioSDKLibrary::Filesize_ToString(BytesPerSecond, MinDecimalsSpeed, MaxDecimalsSpeed);
		if (SpeedTextBlock)
		{
			SpeedTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s/s"), *TotalText.ToString())));
		}
	}
}

void UModioCommonModOperationTrackerUserWidget::UpdateQueuedOperationNumber_Implementation()
{
	if (QueuedOperationNumberTextBlock)
	{
		QueuedOperationNumberTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), GetNumOfQueuedMods())));
	}
}