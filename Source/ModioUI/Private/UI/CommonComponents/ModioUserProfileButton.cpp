/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioUserProfileButton.h"
#include "Core/Input/ModioInputKeys.h"
#include "UObject/StrongObjectPtr.h"

void UModioUserProfileButton::OnButtonClicked()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->SendCommandToBrowserUI(FModioInputKeys::DownloadQueue, 0);
	}
}

void UModioUserProfileButton::NativePreConstruct()
{
	UModioModManagementWidgetBase::NativePreConstruct();
	// We don't need to update the brush here in a live context because the initial value should have been set during
	// edit-time
	if (IsDesignTime() && ProfileImage && NoUserBrushMaterial)
	{
		ProfileImage->SetBrushFromMaterial(NoUserBrushMaterial);
	}
	if (ProfileButton)
	{
		ProfileButton->SetButtonStyle(FModioUIStyleRef {"UserProfileButtonStyle"});
	}
}

void UModioUserProfileButton::NativeOnInitialized()
{
	UModioModManagementWidgetBase::NativeOnInitialized();
	// Register the avatar changed event handler first, because we'll kick off one of those events immediately if
	// there's a user when we register for user changed events
	IModioUIUserAvatarDownloadCompletedReceiver::Register<UModioUserProfileButton>();
	IModioUIUserChangedReceiver::Register<UModioUserProfileButton>();
	if (ProfileButton)
	{
		ProfileButton->OnClicked.AddUniqueDynamic(this, &UModioUserProfileButton::OnButtonClicked);
	}
}

void UModioUserProfileButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// Force update this widget without checking the data source (because the user profile progress indicator should be
	// shown for the current operation regardless of it's mod ID)
	if (!UpdateProgressIfNeeded(true))
	{
		// if requesting a forced update still didn't update anything (because there was nothing in progress) display 0
		// progress
		if (DownloadProgressIndicator)
		{
			DownloadProgressIndicator->SetProgress(0);
		}
	}
}

void UModioUserProfileButton::UpdateProgress(const struct FModioModProgressInfo& ProgressInfo)
{
	UModioModManagementWidgetBase::UpdateProgress(ProgressInfo);

	if (!CurrentUser.IsSet() && DownloadProgressIndicator)
	{
		DownloadProgressIndicator->SetProgress(0);
		return;
	}

	if (DownloadProgressIndicator)
	{
		switch (ProgressInfo.GetCurrentState())
		{
			case EModioModProgressState::Downloading:
				DownloadProgressIndicator->SetProgress(
					(float) ProgressInfo.GetCurrentProgress(EModioModProgressState::Downloading) /
					(float) ProgressInfo.GetTotalProgress(EModioModProgressState::Downloading));
				break;
			case EModioModProgressState::Extracting:
				DownloadProgressIndicator->SetProgress(
					(float) ProgressInfo.GetCurrentProgress(EModioModProgressState::Extracting) /
					(float) ProgressInfo.GetTotalProgress(EModioModProgressState::Extracting));
				break;
			default:
				DownloadProgressIndicator->SetProgress(0);
		}
	}
}

void UModioUserProfileButton::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	CurrentUser = NewUser;

	if (ProfileImage && NoUserBrushMaterial)
	{
		ProfileImage->SetBrushFromSoftMaterial(NoUserBrushMaterial);
	}

	if (NewUser)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->RequestUserAvatar();
		}
	}
}

void UModioUserProfileButton::NativeDestruct()
{
	UModioModManagementWidgetBase::NativeDestruct();
}

void UModioUserProfileButton::NativeOnUserAvatarDownloadCompleted(FModioErrorCode ec,
																  TOptional<FModioImageWrapper> Image)
{
	IModioUIUserAvatarDownloadCompletedReceiver::NativeOnUserAvatarDownloadCompleted(ec, Image);
	if (!ec && ProfileImage)
	{
		if (!UserBrushMaterial)
		{
			ProfileImage->LoadImageFromFileAsync(Image.GetValue());
		}
		else
		{
			GCMaterial = UserBrushMaterial;

			if (GCMaterial)
			{
				ProfileImage->LoadImageFromFileWithMaterialAsync(Image.GetValue(), GCMaterial,
																 ProfileTextureParameterName);
			}
		}
	}
	else if (ProfileImage)
	{
		ProfileImage->SetBrushFromMaterial(NoUserBrushMaterial);
	}
}
