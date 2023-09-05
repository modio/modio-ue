/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioModManagementWidgetBase.h"
#include "Core/ModioModInfoUI.h"
#include "Engine/Engine.h"
#include "ModioSubsystem.h"
#include "ModioUISubsystem.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"

void UModioModManagementWidgetBase::NativeOnInitialized()
{
	IModioUIModManagementEventReceiver::Register<UModioModManagementWidgetBase>();
	IModioUISubscriptionsChangedReceiver::Register<UModioModManagementWidgetBase>();
}

void UModioModManagementWidgetBase::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	if (DataSource)
	{
		UModioModInfoUI* Data = Cast<UModioModInfoUI>(DataSource);
		if (Data)
		{
			if (Data->Underlying.ModId == ModID)
			{
				BeginTickIfNeeded(bNewSubscriptionState);
				SetVisibility(bNewSubscriptionState ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
			}
		}
	}

	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
}

void UModioModManagementWidgetBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
#if WITH_EDITOR
	if (IsDesignTime())
	{
		PreviewProgressInfo =
			FModioModProgressInfo::ConstructPreviewProgressInfo(PreviewProgressState, PreviewCurrent, PreviewTotal);
		PreviewProgressInfo.ID = PreviewModID;
		UpdateProgress(PreviewProgressInfo);
		return;
	}
#endif
}

void UModioModManagementWidgetBase::BeginTickIfNeeded(bool bShouldStartTicking)
{
	/*bool bWasTicking = bShouldTick;
	bShouldTick = bShouldStartTicking;*/
}

void UModioModManagementWidgetBase::UpdateProgress(const FModioModProgressInfo& ProgressInfo) {}

void UModioModManagementWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
	UpdateProgressIfNeeded();
}

bool UModioModManagementWidgetBase::UpdateProgressIfNeeded(bool bForceUpdate)
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		TOptional<FModioModProgressInfo> ProgressInfo = Subsystem->QueryCurrentModUpdate();
		if (ProgressInfo.IsSet())
		{
			if (bForceUpdate)
			{
				UpdateProgress(*ProgressInfo);
				return true;
			}
			else if (DataSource && DataSource->Implements<UModioModInfoUIDetails>())
			{
				FModioModID CurrentModID = IModioModInfoUIDetails::Execute_GetModID(DataSource);
				if (CurrentModID == ProgressInfo->ID)
				{
					UpdateProgress(*ProgressInfo);
					return true;
				}
			}
		}
	}
	return false;
}