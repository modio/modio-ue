/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Utilities/ModOperationTracker/ModioCommonModOperationTrackerWidget.h"
#include "ModioSubsystem.h"
#include "ModioUI.h"
#include "ModioUISubsystem.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"

UModioCommonModOperationTrackerWidget::UModioCommonModOperationTrackerWidget()
{
	UWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UModioCommonModOperationTrackerWidget::SetTrackingModID(FModioModID ModID)
{
	TrackingModID = ModID;
}

void UModioCommonModOperationTrackerWidget::SetTrackAnyMods(bool bInTrackAnyMods)
{
	bTrackAnyMods = bInTrackAnyMods;
}

FModioModID UModioCommonModOperationTrackerWidget::GetModID_Implementation()
{
	if (TrackingModID.IsSet())
	{
		return TrackingModID.GetValue();
	}
	return FModioModID();
}

FModioModInfo UModioCommonModOperationTrackerWidget::GetFullModInfo_Implementation()
{
	UE_LOG(ModioUI, Error, TEXT("Mod operation tracker widget '%s' does not implement GetFullModInfo"), *GetName());
	return IModioModInfoUIDetails::GetFullModInfo_Implementation();
}

bool UModioCommonModOperationTrackerWidget::IsModSubscribed_Implementation() const
{
#if WITH_EDITOR
	if (IsDesignTime())
	{
		if (PreviewModState == EModioModState::Installed)
		{
			return true;
		}
		return false;
	}
#endif

	const FModioModID ModID = Execute_GetModID(const_cast<UModioCommonModOperationTrackerWidget*>(this));
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		return Subsystem->QueryUserSubscriptions().Contains(ModID);
	}

	return false;
}

bool UModioCommonModOperationTrackerWidget::IsModDownloading_Implementation() const
{
#if WITH_EDITOR
	if (IsDesignTime())
	{
		if (PreviewModState == EModioModState::Downloading)
		{
			return true;
		}
		return false;
	}
#endif

	const FModioModID ModID = Execute_GetModID(const_cast<UModioCommonModOperationTrackerWidget*>(this));
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return false;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& UserSubscriptions = Subsystem->QueryUserSubscriptions();

	if (const FModioModCollectionEntry* FoundEntryPtr = UserSubscriptions.Find(ModID))
	{
		const FModioModCollectionEntry& FoundEntry = *FoundEntryPtr;
		return FoundEntry.GetModState() == EModioModState::Downloading;
	}

	return false;
}

bool UModioCommonModOperationTrackerWidget::IsModExtracting_Implementation() const
{
#if WITH_EDITOR
	if (IsDesignTime())
	{
		if (PreviewModState == EModioModState::Extracting)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
#endif

	const FModioModID ModID = Execute_GetModID(const_cast<UModioCommonModOperationTrackerWidget*>(this));
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return false;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& UserSubscriptions = Subsystem->QueryUserSubscriptions();

	if (const FModioModCollectionEntry* FoundEntryPtr = UserSubscriptions.Find(ModID))
	{
		const FModioModCollectionEntry& FoundEntry = *FoundEntryPtr;
		return FoundEntry.GetModState() == EModioModState::Extracting;
	}

	return false;
}

bool UModioCommonModOperationTrackerWidget::IsModInQueue_Implementation() const
{
#if WITH_EDITOR
	if (IsDesignTime())
	{
		if (PreviewModState == EModioModState::InstallationPending || PreviewModState == EModioModState::UpdatePending
			|| PreviewModState == EModioModState::UninstallPending)
		{
			return true;
		}
		return false;
	}
#endif

	const FModioModID ModID = Execute_GetModID(const_cast<UModioCommonModOperationTrackerWidget*>(this));
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return false;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& UserSubscriptions = Subsystem->QueryUserSubscriptions();

	if (const FModioModCollectionEntry* FoundEntryPtr = UserSubscriptions.Find(ModID))
	{
		const FModioModCollectionEntry& FoundEntry = *FoundEntryPtr;
		return FoundEntry.GetModState() == EModioModState::UpdatePending
		|| FoundEntry.GetModState() == EModioModState::InstallationPending
		|| FoundEntry.GetModState() == EModioModState::UninstallPending;
	}

	return false;
}

bool UModioCommonModOperationTrackerWidget::IsModEnabled_Implementation()
{
#if WITH_EDITOR
	if (IsDesignTime())
	{
		return bPreviewIsModEnabled;
	}
#endif

	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (!Subsystem->GetIsCollectionModDisableUIEnabled())
		{
			UE_LOG(ModioUI, Verbose, TEXT("Mod enable/disable functionality is disabled, returning true for IsModEnabled"));
			return true;
		}
	}

	const FModioModID ModID = Execute_GetModID(this);
	return QueryModEnabled(ModID);
}

bool UModioCommonModOperationTrackerWidget::IsModInstalled_Implementation() const
{
#if WITH_EDITOR
	if (IsDesignTime())
	{
		if (PreviewModState == EModioModState::Installed)
		{
			return true;
		}
		return false;
	}
#endif

	const FModioModID ModID = Execute_GetModID(const_cast<UModioCommonModOperationTrackerWidget*>(this));
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return false;
	}

	const TMap<FModioModID, FModioModCollectionEntry>& UserSubscriptions = Subsystem->QueryUserSubscriptions();

	if (const FModioModCollectionEntry* FoundEntryPtr = UserSubscriptions.Find(ModID))
	{
		const FModioModCollectionEntry& FoundEntry = *FoundEntryPtr;
		return FoundEntry.GetModState() == EModioModState::Installed;
	}

	return false;
}

bool UModioCommonModOperationTrackerWidget::HasModErrors_Implementation() const
{
#if WITH_EDITOR
	if (IsDesignTime())
	{
		return bPreviewHasModErrors;
	}
#endif
	return bHasErrors;
}

int64 UModioCommonModOperationTrackerWidget::GetPreviewTotal() const
{
#if WITH_EDITOR
	return PreviewTotal;
#else
	return 0;
#endif
}

int64 UModioCommonModOperationTrackerWidget::GetPreviewCurrent() const
{
#if WITH_EDITOR
	return PreviewCurrent;
#else
	return 0;
#endif
}

void UModioCommonModOperationTrackerWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	IModioUIModManagementEventReceiver::Register<UModioCommonModOperationTrackerWidget>();
	IModioUIModEnableWidget::Register<UModioCommonModOperationTrackerWidget>();
}

void UModioCommonModOperationTrackerWidget::Tick(float DeltaTime)
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		TOptional<FModioModProgressInfo> ProgressInfo = Subsystem->QueryCurrentModUpdate();
		if (ProgressInfo.IsSet() && (bTrackAnyMods || (TrackingModID.IsSet() && TrackingModID == ProgressInfo->ID)))
		{
			UpdateProgress(*ProgressInfo);
			PreviousUpdateTime = FDateTime::Now();
			return;
		}
	}
	HideProgress();
}

void UModioCommonModOperationTrackerWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UModioCommonModOperationTrackerWidget::UpdateProgress(const FModioModProgressInfo& ProgressInfo)
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return;
	}

	if (!bTrackAnyMods)
	{
#if WITH_EDITOR
		if (!IsDesignTime())
#endif
		{
			const bool bCurrentlyUpdatingThisMod = TrackingModID.IsSet() && Subsystem->QueryCurrentModUpdate().IsSet() &&
												   Subsystem->QueryCurrentModUpdate()->ID == TrackingModID;
			if (!bCurrentlyUpdatingThisMod)
			{
				HideProgress();
				return;
			}
		}
	}
	
	const EModioModProgressState CurrentState = ProgressInfo.GetCurrentState();
	if (CurrentState == EModioModProgressState::Downloading || CurrentState == EModioModProgressState::Extracting)
	{
		const FTimespan FTS = FDateTime::Now() - PreviousUpdateTime;
		const double DeltaTime = FTS.GetTotalSeconds();

		const FModioUnsigned64 Current = ProgressInfo.GetCurrentProgress(CurrentState);
		const FModioUnsigned64 Total = ProgressInfo.GetTotalProgress(CurrentState);

		BroadcastProgress(Current, Total);
		BroadcastSpeed(Current - PreviousProgressValue, DeltaTime);

		PreviousProgressValue = Current;
	}
	else
	{
		HideProgress();
	}
}

void UModioCommonModOperationTrackerWidget::HideProgress()
{
	if (PreviousProgressValue.Underlying != 0)
	{
		PreviousProgressValue = FModioUnsigned64(0);
		BroadcastProgress(FModioUnsigned64(0), FModioUnsigned64(0));
		BroadcastSpeed(FModioUnsigned64(0), 0);
	}
}

void UModioCommonModOperationTrackerWidget::BroadcastProgress(const FModioUnsigned64& Current, const FModioUnsigned64& Total)
{
	OnProgress.Broadcast(Current, Total);
	OnProgressFast.Broadcast(Current, Total);
}

void UModioCommonModOperationTrackerWidget::BroadcastSpeed(const FModioUnsigned64& DeltaBytes, double DeltaTime)
{
	OnSpeed.Broadcast(DeltaBytes, DeltaTime);
	OnSpeedFast.Broadcast(DeltaBytes, DeltaTime);
}

void UModioCommonModOperationTrackerWidget::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	const FModioModID CurrentModID = Execute_GetModID(this);
	if (CurrentModID == Event.ID)
	{
		if (Event.Status)
		{
			bHasErrors = true;
		}
	}

	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);
}
