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

#include "Types/ModioUser.h"
#include "UObject/Interface.h"

#include "IModioUIDownloadQueueWidget.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIDownloadQueueWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIDownloadQueueWidget : public IInterface
{
	GENERATED_BODY()
protected:
	virtual void NativeDisplayUserInfo(FModioOptionalUser NewUser) {};
	virtual void NativeRefreshDownloadQueue() {};

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIDownloadQueueWidget")
	void DisplayUserInfo(FModioOptionalUser NewUser);

	void DisplayUserInfo_Implementation(FModioOptionalUser NewUser)
	{
		NativeDisplayUserInfo(NewUser);
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIDownloadQueueWidget")
	void RefreshDownloadQueue();

	void RefreshDownloadQueue_Implementation()
	{
		NativeRefreshDownloadQueue();
	}
};