/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioImageWrapper.h"
#include "Types/ModioUser.h"
#include "UObject/Interface.h"

#include "IModioUIUserAvatarDownloadCompletedReceiver.generated.h"

UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioUIUserAvatarDownloadCompletedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIUserAvatarDownloadCompletedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedAvatarDownloaded = false;
	void UserAvatarDownloadCompletedHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image);

protected:
	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->OnUserAvatarDownloadCompleted.AddUObject(
				Cast<ImplementingClass>(this),
				&IModioUIUserAvatarDownloadCompletedReceiver::UserAvatarDownloadCompletedHandler);
		}
	}

	virtual void NativeOnUserAvatarDownloadCompleted(FModioErrorCode ec, TOptional<FModioImageWrapper> Image);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUserAvatarDownloadCompleted(FModioErrorCode ec, FModioOptionalImage Image);
};
