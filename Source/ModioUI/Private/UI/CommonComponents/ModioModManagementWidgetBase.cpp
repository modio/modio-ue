// Fill out your copyright notice in the Description page of Project Settings.

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
		PreviewProgressInfo = FModioModProgressInfo {
			FModioUnsigned64(TotalDownloadSize), FModioUnsigned64(CurrentDownloadSize),
			FModioUnsigned64(TotalExtractedSize), FModioUnsigned64(CurrentExtractedSize), PreviewModID};
		UpdateProgress(PreviewProgressInfo);
		return;
	}
#endif
}

void UModioModManagementWidgetBase::BeginTickIfNeeded(bool bShouldStartTicking)
{
	bool bWasTicking = bShouldTick;
	bShouldTick = bShouldStartTicking;
	// Currently not used because UUserWidgets always tick
	/*if (bShouldTick && !bWasTicking)
	{
		RegisterActiveTimer(0.0f,
							FWidgetActiveTimerDelegate::CreateUObject(this, &UModioSubscriptionBadge::RegisteredTick));
	}*/
}

void UModioModManagementWidgetBase::UpdateProgress(const FModioModProgressInfo& ProgressInfo) {}

void UModioModManagementWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);

	if (bShouldTick)
	{
		UpdateProgressIfNeeded();
	}
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
