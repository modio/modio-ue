// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Drawers/DownloadQueue/ModioDownloadQueueOpProgress.h"
#include "Core/ModioModInfoUI.h"
#include "Types/ModioModProgressInfo.h"

void UModioDownloadQueueOpProgress::UpdateSpeed(FModioUnsigned64 DeltaBytes, double DeltaTime)
{
	if (DeltaBytes)
	{
		int64 BytesPerSecond = DeltaBytes / DeltaTime;
		// UE_LOG(LogTemp, Display, TEXT("%lld bytes"), BytesPerSecond);
		FText BytesPerSecondString = FText::AsMemory(BytesPerSecond, EMemoryUnitStandard::IEC);
		OperationSpeedText->SetText(FText::Format(
			SpeedFormatText, FFormatNamedArguments {{FString("Bytes"), FFormatArgumentValue(BytesPerSecondString)}}));
	}
}

void UModioDownloadQueueOpProgress::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	UModioModInfoUI* Data = Cast<UModioModInfoUI>(DataSource);
	if (Data)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			if (TOptional<FModioModProgressInfo> ProgressInfo = Subsystem->QueryCurrentModUpdate())
			{
				if (ProgressInfo->ID == Data->Underlying.ModId)
				{
					SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
		}
		ModNameLabel->SetText(FText::FromString(Data->Underlying.ProfileName));
	}
	BeginTickIfNeeded(true);
}

void UModioDownloadQueueOpProgress::SetPercent(float InPercent)
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(InPercent);
	}
}

void UModioDownloadQueueOpProgress::UpdateProgress(const struct FModioModProgressInfo& ProgressInfo)
{
	FTimespan fts = FDateTime::Now() - PreviousUpdateTime;
	double DeltaTime = fts.GetTotalSeconds();

	if (ProgressInfo.CurrentlyDownloadedBytes < ProgressInfo.TotalDownloadSize)
	{
		SetPercent(ProgressInfo.CurrentlyDownloadedBytes / (double) ProgressInfo.TotalDownloadSize);
		UpdateSpeed(ProgressInfo.CurrentlyDownloadedBytes - PreviousProgressValue, DeltaTime);

		PreviousProgressValue = ProgressInfo.CurrentlyDownloadedBytes;
	}
	else if (ProgressInfo.CurrentlyExtractedBytes < ProgressInfo.TotalExtractedSizeOnDisk)
	{
		SetPercent(ProgressInfo.CurrentlyExtractedBytes / (double) ProgressInfo.TotalExtractedSizeOnDisk);
		UpdateSpeed(ProgressInfo.CurrentlyExtractedBytes - PreviousProgressValue, DeltaTime);
		PreviousProgressValue = ProgressInfo.CurrentlyExtractedBytes;
	}
	else
	{
		PreviousProgressValue = FModioUnsigned64(0);
	}

	PreviousUpdateTime = FDateTime::Now();
}

void UModioDownloadQueueOpProgress::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);
	if (DataSource)
	{
		UModioModInfoUI* Data = Cast<UModioModInfoUI>(DataSource);
		if (Data)
		{
			if (Data->Underlying.ModId == Event.ID)
			{
				if (Event.Status)
				{
					// Display badge for error?
				}
				else
				{
					switch (Event.Event)
					{
						case EModioModManagementEventType::Installed:
						case EModioModManagementEventType::Updated:
							// hide/collapse the widget?
							/*if (Label)
							{
								Label->SetText(SubscribedLabelText);
							}
							SetPercent(100.f);*/
							break;
					}
				}
				// We don't need to keep ticking, success or fail
				bShouldTick = false;
				OnOperationCompleted.ExecuteIfBound();
			}
		}
	}
}

FOnDownloadOpComplete& UModioDownloadQueueOpProgress::OperationCompletedDelegate()
{
	return OnOperationCompleted;
}
