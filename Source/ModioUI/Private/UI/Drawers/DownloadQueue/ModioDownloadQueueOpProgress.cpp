/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Drawers/DownloadQueue/ModioDownloadQueueOpProgress.h"
#include "Core/ModioModInfoUI.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Types/ModioModProgressInfo.h"

void UModioDownloadQueueOpProgress::UpdateSpeed(FModioUnsigned64 DeltaBytes, double DeltaTime)
{
	if (DeltaBytes)
	{
		//int64 BytesPerSecond = DeltaBytes / DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("%lld bytes"), BytesPerSecond);
		//FText BytesPerSecondString = FText::AsMemory(BytesPerSecond, EMemoryUnitStandard::SI);

		int64 BytesPerSecond = DeltaBytes / DeltaTime / 8;
		FText BytesPerSecondString = FText::AsMemory(BytesPerSecond, EMemoryUnitStandard::SI);


		FText RoundedBytesPerSecondString = UModioSDKLibrary::RoundNumberString(BytesPerSecondString);
		OperationSpeedText->SetText(FText::Format(
			SpeedFormatText,
			FFormatNamedArguments {{FString("Bytes"), FFormatArgumentValue(RoundedBytesPerSecondString)}}));
		OperationSpeedText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	}
}

void UModioDownloadQueueOpProgress::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (UnsubscribeButton)
	{
		UnsubscribeButton->OnClicked.AddDynamic(this, &UModioDownloadQueueOpProgress::OnUnsubscribeClicked);
	}
}

void UModioDownloadQueueOpProgress::OnUnsubscribeClicked()
{
	UModioModInfoUI* ActualData = Cast<UModioModInfoUI>(DataSource);
	if (ActualData)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->ShowModUnsubscribeDialog(ActualData);
		}
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
	switch (ProgressInfo.GetCurrentState())
	{
		case EModioModProgressState::Downloading:
		{
			FModioUnsigned64 Current = ProgressInfo.GetCurrentProgress(EModioModProgressState::Downloading);
			FModioUnsigned64 Total = ProgressInfo.GetTotalProgress(EModioModProgressState::Downloading);

			SetPercent(Current / (double) Total);
			UpdateSpeed(Current - PreviousProgressValue, DeltaTime);

			PreviousProgressValue = Current;
			int decimals = 0;
			Current.Underlying < 1024 * 1024 * 1024 ? decimals = 0 : decimals = 1;

			FFormatNamedArguments Args;
			Args.Add("Progress",
					 UModioSDKLibrary::Filesize_ToString(Current.Underlying, decimals));
			Args.Add("Total", UModioSDKLibrary::Filesize_ToString(Total.Underlying, 1));

			FText currenttext = UModioSDKLibrary::Filesize_ToString(Current.Underlying, 1);
			FText totaltext = UModioSDKLibrary::Filesize_ToString(Total.Underlying, 1);

			OperationProgressText->SetText(FText::Format(FTextFormat(ProgressFormatText), Args));
			OperationProgressText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		break;
		case EModioModProgressState::Extracting:
		{
			FModioUnsigned64 Current = ProgressInfo.GetCurrentProgress(EModioModProgressState::Downloading);
			FModioUnsigned64 Total = ProgressInfo.GetTotalProgress(EModioModProgressState::Downloading);

			SetPercent(Current / (double) Total);

			OperationProgressText->SetVisibility(ESlateVisibility::Collapsed);
			OperationSpeedText->SetVisibility(ESlateVisibility::Collapsed);
			PreviousProgressValue = Current;
		}
		break;
		default:
		{
			PreviousProgressValue = FModioUnsigned64(0);
		}
		break;
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
