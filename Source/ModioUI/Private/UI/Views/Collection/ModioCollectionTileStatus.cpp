/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/Collection/ModioCollectionTileStatus.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "Core/ModioModInfoUI.h"
#include "Libraries/ModioErrorConditionLibrary.h"

void UModioCollectionTileStatus::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (!DataSource)
	{
		return;
	}

	UModioModCollectionEntryUI* CollectionEntry = Cast<UModioModCollectionEntryUI>(DataSource);
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();

	if (!IsValid(CollectionEntry) || !Subsystem)
	{
		return;
	}

	TMap<FModioModID, FModioModCollectionEntry> UserMods = Subsystem->QueryUserSubscriptions();
	if (!UserMods.Contains(CollectionEntry->Underlying.GetID()))
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	ProgressBarSizeBox->SetVisibility(ESlateVisibility::Collapsed);
	StatusPercent->SetVisibility(ESlateVisibility::Collapsed);

	FModioErrorCode ec = UserMods[CollectionEntry->Underlying.GetID()].GetLastError();

	if (ec)
	{
		if (StatusText)
		{
			StatusText->SetDefaultStyleName("error");
			StatusText->SynchronizeProperties();

			if (UModioErrorConditionLibrary::ErrorCodeMatches(ec, EModioErrorCondition::InsufficientSpace))
			{
				StatusText->SetText(NSLOCTEXT("Modio", "InsufficientSpaceError", "Insufficient disk space"));
			} else
			{
				StatusText->SetText(FText::FromString(ec.GetErrorMessage()));	
			}
			
			SetPercent(0.0f);
		}
	} else
	{
		StatusText->SetDefaultStyleName("default");
		StatusText->SynchronizeProperties();
		switch (UserMods[CollectionEntry->Underlying.GetID()].GetModState())
		{
			case EModioModState::InstallationPending:
				StatusText->SetText(PendingLabelText);
				break;
			case EModioModState::UninstallPending:
				StatusText->SetText(UninstallPendingLabelText);
				break;
			case EModioModState::UpdatePending:
				StatusText->SetText(UpdatePendingLabelText);
				break;
			case EModioModState::Downloading:
				StatusText->SetText(DownloadingLabelText);
				break;
			case EModioModState::Extracting:
				StatusText->SetText(ExtractingLabelText);
				BeginTickIfNeeded(true);
				break;
			case EModioModState::Installed:
				StatusText->SetText(InstalledLabelText);
				break;
			default:
				SetPercent(0.0f);
				break;
		}
	}

}

void UModioCollectionTileStatus::SetPercent(float InPercent)
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(InPercent);
	}
}

void UModioCollectionTileStatus::UpdateProgress(const struct FModioModProgressInfo& ProgressInfo)
{
	switch (ProgressInfo.GetCurrentState())
	{
		case EModioModProgressState::Downloading:
		{
			StatusText->SetText(DownloadingLabelText);
			if (ProgressBarSizeBox && ProgressBarSizeBox->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
			{
				ProgressBarSizeBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			if (StatusPercent && StatusPercent->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
			{
				StatusPercent->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			float Current = (float) ProgressInfo.GetCurrentProgress(EModioModProgressState::Downloading);
			float Total = (float) ProgressInfo.GetTotalProgress(EModioModProgressState::Downloading);
			int percent = Current / Total * 100;
			StatusPercent->SetText(FText::FromString(FString::FromInt(percent).Append(" %")));
			SetPercent(Current / Total);
		}
		break;
		case EModioModProgressState::Extracting:
		{
			StatusText->SetText(ExtractingLabelText);
			float Current = (float) ProgressInfo.GetCurrentProgress(EModioModProgressState::Extracting);
			float Total = (float) ProgressInfo.GetTotalProgress(EModioModProgressState::Extracting);
			int percent = Current / Total * 100;
			StatusPercent->SetText(FText::FromString(FString::FromInt(percent).Append(" %")));
			SetPercent(Current / Total);
		}
		break;
	}
}

void UModioCollectionTileStatus::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);
	if (DataSource)
	{
		UModioModCollectionEntryUI* Data = Cast<UModioModCollectionEntryUI>(DataSource);
		if (Data)
		{
			if (Data->Underlying.GetID() == Event.ID)
			{
				if (Event.Status)
				{
					if (StatusText)
					{
						StatusText->SetDefaultStyleName("error");
						StatusText->SynchronizeProperties();

						if (UModioErrorConditionLibrary::ErrorCodeMatches(Event.Status,
																		  EModioErrorCondition::InsufficientSpace))
						{
							StatusText->SetText(
								NSLOCTEXT("Modio", "InsufficientSpaceError", "Insufficient disk space"));
						}
						else
						{
							StatusText->SetText(FText::FromString(Event.Status.GetErrorMessage()));
						}

						if (ProgressBarSizeBox)
						{
							ProgressBarSizeBox->SetVisibility(ESlateVisibility::Hidden);
						}

						if (StatusPercent)
						{
							StatusPercent->SetVisibility(ESlateVisibility::Hidden);
						}
					}
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
			}
		}
	}
}
