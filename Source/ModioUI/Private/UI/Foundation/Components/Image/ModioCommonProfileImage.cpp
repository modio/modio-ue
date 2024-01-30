/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Image/ModioCommonProfileImage.h"

void UModioCommonProfileImage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// Register the avatar changed event handler first, because we'll kick off one of those events immediately if
	// there's a user when we register for user changed events
	IModioUIUserAvatarDownloadCompletedReceiver::Register<UModioCommonProfileImage>();
	IModioUIUserChangedReceiver::Register<UModioCommonProfileImage>();
}

void UModioCommonProfileImage::NativeOnUserAvatarDownloadCompleted(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> InImage)
{
	IModioUIUserAvatarDownloadCompletedReceiver::NativeOnUserAvatarDownloadCompleted(ErrorCode, InImage);

	if (ErrorCode || !InImage.IsSet())
	{
		OnLoadNewImageError();
		return;
	}

	LoadImageFromFileAsync(InImage.GetValue());
}

void UModioCommonProfileImage::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	LoadedImageTexture = nullptr;
	OnStartLoadNewImage();

	if (NewUser)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->RequestUserAvatar();
		}
	}
}
