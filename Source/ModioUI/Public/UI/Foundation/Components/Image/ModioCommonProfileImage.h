/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "ModioCommonDynamicImage.h"
#include "UI/EventHandlers/IModioUIUserAvatarDownloadCompletedReceiver.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "ModioCommonProfileImage.generated.h"

/**
 * Image that displays a user's avatar
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonProfileImage
	: public UModioCommonDynamicImage,
	  public IModioUIUserChangedReceiver,
	  public IModioUIUserAvatarDownloadCompletedReceiver
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	//~ Begin IModioUIUserAvatarDownloadCompletedReceiver Interface
	virtual void NativeOnUserAvatarDownloadCompleted(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> InImage) override;
	//~ End IModioUIUserAvatarDownloadCompletedReceiver Interface

	//~ Begin IModioUIUserChangedReceiver Interface
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	//~ End IModioUIUserChangedReceiver Interface
};
