/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioSubscriptionBadge.h"
#include "Core/ModioModInfoUI.h"
#include "Engine/Engine.h"
#include "ModioSubsystem.h"
#include "Settings/ModioUISettings.h"
#include "Slate/SRetainerWidget.h"
#include "UI/Styles/ModioSubscriptionBadgeStyle.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

void UModioSubscriptionBadge::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (!DataSource)
	{
		return;
	}

	UModioModInfoUI* Data = Cast<UModioModInfoUI>(DataSource);
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Data || !Subsystem)
	{
		return;
	}

	TMap<FModioModID, FModioModCollectionEntry> UserMods = Subsystem->QueryUserSubscriptions();
	if (!UserMods.Contains(Data->Underlying.ModId))
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	switch (UserMods[Data->Underlying.ModId].GetModState())
	{
		case EModioModState::InstallationPending:
			Label->SetText(PendingLabelText);
		case EModioModState::UninstallPending:
		case EModioModState::UpdatePending:
			Label->SetText(PendingLabelText);
			SetPercent(0.f);
			break;
		case EModioModState::Downloading:
			Label->SetText(DownloadingLabelText);
		case EModioModState::Extracting:
			Label->SetText(ExtractingLabelText);
			BeginTickIfNeeded(true);
			break;
		case EModioModState::Installed:
			Label->SetText(SubscribedLabelText);
			SetPercent(100.f);
			break;
	}

	SetVisibility(UserMods.Contains(Data->Underlying.ModId) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UModioSubscriptionBadge::SetPercent(float InPercent)
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(InPercent);
	}
}


void UModioSubscriptionBadge::UpdateProgress(const struct FModioModProgressInfo& ProgressInfo)
{
	switch (ProgressInfo.GetCurrentState())
	{
		case EModioModProgressState::Downloading:
			Label->SetText(DownloadingLabelText);
			SetPercent(ProgressInfo.GetCurrentProgress(EModioModProgressState::Downloading) /
					   (double) ProgressInfo.GetTotalProgress(EModioModProgressState::Downloading));
			break;
		case EModioModProgressState::Extracting:
			Label->SetText(ExtractingLabelText);
			SetPercent(ProgressInfo.GetCurrentProgress(EModioModProgressState::Extracting) /
					   (double) ProgressInfo.GetTotalProgress(EModioModProgressState::Extracting));
			break;
		case EModioModProgressState::Initializing:

			break;
		case EModioModProgressState::Compressing:
			break;
	}
}

void UModioSubscriptionBadge::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	WrappedContent.Reset();
}

void UModioSubscriptionBadge::NativeOnModManagementEvent(FModioModManagementEvent Event)
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
					SetPercent(0.f);
				}
				else
				{
					switch (Event.Event)
					{
						case EModioModManagementEventType::Installed:
						case EModioModManagementEventType::Updated:
							if (Label)
							{
								Label->SetText(SubscribedLabelText);
							}
							SetPercent(100.f);
							break;
					}
				}
				// We don't need to keep ticking, success or fail
				bShouldTick = false;
			}
		}
	}
}

void UModioSubscriptionBadge::SynchronizeProperties()
{
	const FModioSubscriptionBadgeStyle* ResolvedStyle = BadgeStyle.FindStyle<FModioSubscriptionBadgeStyle>();
	if (ResolvedStyle)
	{
		if (ProgressBar)
		{
			ProgressBar->SetStyleRef(ResolvedStyle->ProgressBarStyle);
		}
		if (Label)
		{
			Label->GetStyleDelegate().BindUObject(this, &UModioSubscriptionBadge::GetLabelStyle);
		}
		// apply the text label style here
	}
	/*
		UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>();
		// Could we load the default style set?
		if (Settings && !Settings->DefaultStyleSet.IsNull())
		{
			UModioUIStyleSet* DefaultStyle = Settings->DefaultStyleSet.LoadSynchronous();
			FModioProgressBarStyle* ResolvedStyle = &DefaultStyle->DefaultProgressBarStyle;
			if (ProgressBar)
			{
				ProgressBar->WidgetStyle = *ResolvedStyle;

				FModioSubscriptionBadgeStyle* DefaultBadgeStyle = &DefaultStyle->DefaultSubscriptionBadgeStyle;
				if (DefaultBadgeStyle && DefaultBadgeStyle->BadgeBorderStyle.bMaskWithMaterial)
				{
					if (!WrappedContent || !WrappedContent.IsValid())
					{
						SAssignNew(WrappedContent, SRetainerWidget)[ProgressBar->TakeWidget()];
					}
					WrappedContent->SetTextureParameter("WidgetTexture");
					WrappedContent->SetEffectMaterial(DefaultBadgeStyle->BadgeBorderStyle.MaskMaterial.LoadSynchronous());
					ApplyModioBorderStyle(WrappedContent->GetEffectMaterial(), &DefaultBadgeStyle->BadgeBorderStyle);
				}
				if (DefaultBadgeStyle)
				{
					if (Label)
					{
						Label->SetDefaultTextStyle(DefaultBadgeStyle->LabelStyle);
					}
				}
			}
		}*/
	Super::SynchronizeProperties();
}

const FModioRichTextStyle& UModioSubscriptionBadge::GetLabelStyle() const
{
	if (const FModioSubscriptionBadgeStyle* ResolvedStyle = BadgeStyle.FindStyle<FModioSubscriptionBadgeStyle>())
	{
		if (const FModioRichTextStyle* ResolvedLabelStyle =
				ResolvedStyle->LabelStyleNew.FindStyle<FModioRichTextStyle>())
		{
			return *ResolvedLabelStyle;
		}
	}
	return FModioRichTextStyle::GetDefault();
}

TSharedRef<SWidget> UModioSubscriptionBadge::RebuildWidget()
{
	TSharedRef<SWidget> UnderlyingProgressBar = Super::RebuildWidget();
	const FModioSubscriptionBadgeStyle* ResolvedStyle = BadgeStyle.FindStyle<FModioSubscriptionBadgeStyle>();
	if (ResolvedStyle)
	{
		if (ProgressBar)
		{
			ProgressBar->SetStyleRef(ResolvedStyle->ProgressBarStyle);
		}
		// apply the text label style here
		if (Label)
		{
			Label->GetStyleDelegate().BindUObject(this, &UModioSubscriptionBadge::GetLabelStyle);
		}
	}
	// Either couldn't load stylesheet or we didnt want to wrap the progress bar in a retainer
	return UnderlyingProgressBar;
}
