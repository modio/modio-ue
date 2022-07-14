// Fill out your copyright notice in the Description page of Project Settings.

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
	if (IsDesignTime() && ProfileImage && !NoUserBrushMaterial.IsNull())
	{
		ProfileImage->SetBrushFromMaterial(NoUserBrushMaterial.LoadSynchronous());
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
	if (DownloadProgressIndicator)
	{
		if (ProgressInfo.CurrentlyDownloadedBytes < ProgressInfo.TotalDownloadSize)
		{
			DownloadProgressIndicator->SetProgress((float) ProgressInfo.CurrentlyDownloadedBytes /
												   (float) ProgressInfo.TotalDownloadSize);
		}
		else if (ProgressInfo.CurrentlyExtractedBytes < ProgressInfo.TotalExtractedSizeOnDisk)
		{
			DownloadProgressIndicator->SetProgress((float) ProgressInfo.CurrentlyExtractedBytes /
												   (float) ProgressInfo.TotalExtractedSizeOnDisk);
		}
		else
		{
			DownloadProgressIndicator->SetProgress(0);
		}
	}
}

void UModioUserProfileButton::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	CurrentUser = NewUser;

	if (ProfileImage && !NoUserBrushMaterial.IsNull())
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
		if (UserBrushMaterial.IsNull())
		{
			ProfileImage->LoadImageFromFileAsync(Image.GetValue());
		}
		else
		{
			GCMaterial = UserBrushMaterial.LoadSynchronous();

			if (GCMaterial)
			{
				ProfileImage->LoadImageFromFileWithMaterialAsync(Image.GetValue(), GCMaterial,
																 ProfileTextureParameterName);
			}
		}
	}
	else if (ProfileImage)
	{
		ProfileImage->SetBrushFromSoftMaterial(NoUserBrushMaterial);
	}
}
