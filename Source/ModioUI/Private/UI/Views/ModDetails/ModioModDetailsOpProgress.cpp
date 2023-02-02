/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/ModDetails/ModioModDetailsOpProgress.h"
#include "Core/ModioModInfoUI.h"
#include "Types/ModioModProgressInfo.h"

void UModioModDetailsOpProgress::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Hidden);
}

void UModioModDetailsOpProgress::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource))
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			if (TOptional<FModioModProgressInfo> ProgressInfo = Subsystem->QueryCurrentModUpdate())
			{
				if (ProgressInfo->ID == ModInfo->Underlying.ModId)
				{
					SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					BeginTickIfNeeded(true);
				}
			}
		}
	}
}

void UModioModDetailsOpProgress::UpdateSpeed(FModioUnsigned64 DeltaBytes, double DeltaTime)
{
	if (DeltaBytes)
	{
		double BytesPerSecond = DeltaBytes / DeltaTime;
		FText BytesPerSecondString = FText::AsMemory(BytesPerSecond, EMemoryUnitStandard::SI);
		if (OperationSpeedText)
		{
			OperationSpeedText->SetText(
				FText::Format(SpeedTextFormat,
							  FFormatNamedArguments {{FString("Bytes"), FFormatArgumentValue(BytesPerSecondString)}}));
		}
	}
}

void UModioModDetailsOpProgress::SetPercent(float InPercent)
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(InPercent);
	}
	if (OperationProgressText)
	{
		OperationProgressText->SetText(FText::AsPercent(InPercent));
	}
}

void UModioModDetailsOpProgress::UpdateProgress(const struct FModioModProgressInfo& ProgressInfo)
{
	FTimespan fts = FDateTime::Now() - PreviousUpdateTime;
	double DeltaTime = PreviousUpdateTime.GetTicks() ? fts.GetTotalMilliseconds() / 1000 : 0.01;
	switch (ProgressInfo.GetCurrentState())
	{
		case EModioModProgressState::Downloading:
		{
			FModioUnsigned64 Current = ProgressInfo.GetCurrentProgress(EModioModProgressState::Downloading);
			FModioUnsigned64 Total= ProgressInfo.GetTotalProgress(EModioModProgressState::Downloading);
			SetPercent(Current / (double) Total);
			UpdateTimeRemaining(Current - PreviousProgressValue,
								Total - Current, DeltaTime);
			UpdateSpeed(Current - PreviousProgressValue, DeltaTime);
			PreviousProgressValue = Current;
		}
		break;
		case EModioModProgressState::Extracting:
		{
			FModioUnsigned64 Current = ProgressInfo.GetCurrentProgress(EModioModProgressState::Extracting);
			FModioUnsigned64 Total = ProgressInfo.GetTotalProgress(EModioModProgressState::Extracting);
			SetPercent(Current / (double) Total);
			UpdateTimeRemaining(Current - PreviousProgressValue,
								Total - Current,
								DeltaTime);
			UpdateSpeed(Current - PreviousProgressValue, DeltaTime);
			PreviousProgressValue = Current;
		}
		break;
		default:
		{
			if (TimeRemainingText)
			{
				TimeRemainingText->SetText(FText::GetEmpty());
			}
			PreviousProgressValue = FModioUnsigned64(0);
		}
		break;
	}
	
	PreviousUpdateTime = FDateTime::Now();
}

void UModioModDetailsOpProgress::UpdateTimeRemaining(FModioUnsigned64 ProgressSinceLastUpdate,
													 FModioUnsigned64 AmountRemaining, double SecondsSinceLastUpdate)
{
	if (AmountRemaining)
	{
		if (!FMath::IsNearlyZero(SecondsSinceLastUpdate))
		{
			double ProgressPerSecond = ProgressSinceLastUpdate / SecondsSinceLastUpdate;
			if (ProgressPerSecond)
			{
				float TotalSecondsRemaining = AmountRemaining / ProgressPerSecond;
				if (TimeRemainingText)
				{
					TimeRemainingText->SetText(FText::Format(
						TimeRemainingTextFormat, FText::AsTimespan(FTimespan::FromSeconds(TotalSecondsRemaining))));
				}
			}
		}
	}
	else
	{
		if (TimeRemainingText)
		{
			TimeRemainingText->SetText(
				FText::Format(TimeRemainingTextFormat, FText::AsTimespan(FTimespan::FromSeconds(0))));
		}
	}
}

void UModioModDetailsOpProgress::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	Super::NativeOnModManagementEvent(Event);
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
							SetPercent(1);
							UpdateTimeRemaining(FModioUnsigned64(), FModioUnsigned64(), 0);
							SetVisibility(ESlateVisibility::Hidden);
							break;
					}
				}
				// We don't need to keep ticking, success or fail
				bShouldTick = false;
			}
		}
	}
}
