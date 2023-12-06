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

#include "Delegates/Delegate.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "UObject/Interface.h"

#include "IModioUIAsyncRetryWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnRetryRequested);

UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioUIAsyncRetryWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIAsyncRetryWidget : public IInterface
{
	GENERATED_BODY()

protected:
	FOnRetryRequested OnRetryRequestedDelegate;
	void SetRetryRequestedDelegate_Implementation(const FOnRetryRequested& Delegate)
	{
		OnRetryRequestedDelegate = Delegate;
	}

public:
	/// @brief Call this to pass in a delegate that will receive operation state change notifications
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAsyncRetryWidget")
	void SetRetryRequestedDelegate(const FOnRetryRequested& Delegate);
};