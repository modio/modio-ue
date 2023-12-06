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

#include "Delegates/Delegate.h"
#include "UObject/Interface.h"

#include "IModioUIAsyncOperationWidget.generated.h"

/**
* Enumerator with possible states an asynchronus operation can be in
**/
UENUM(BlueprintType)
enum class EModioUIAsyncOperationWidgetState : uint8
{
	/** Operation succeeded **/
	Success,

	/** Operation failed **/
	Error,

	/** Operation still working **/
	InProgress
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSetOperationState, EModioUIAsyncOperationWidgetState, NewState);

UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioUIAsyncOperationWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIAsyncOperationWidget : public IInterface
{
	GENERATED_BODY()

protected:
	friend class UModioAsyncOpWrapperWidget;

	FOnSetOperationState OnStateChangeDelegate;
	bool bRoutedStateChangeNotification = false;
	virtual void NativeNotifyOperationState(EModioUIAsyncOperationWidgetState NewState)
	{
		bRoutedStateChangeNotification = true;
		OnStateChangeDelegate.ExecuteIfBound(NewState);
	};

	virtual void NativeRequestOperationRetry() {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAsyncOperationWidget")
	void NotifyOperationState(EModioUIAsyncOperationWidgetState NewState);

	void NotifyOperationState_Implementation(EModioUIAsyncOperationWidgetState NewState)
	{
		bRoutedStateChangeNotification = false;
		NativeNotifyOperationState(NewState);
		checkf(bRoutedStateChangeNotification,
			   TEXT("Please call IModioUIAsyncOperationWidget::NativeNotifyOperationState in your overridden "
					"implementation to ensure that events are routed correctly"));
	}

	void RequestOperationRetry_Implementation()
	{
		NativeRequestOperationRetry();
	}

	void SetOperationStateDelegate_Implementation(const FOnSetOperationState& Delegate)
	{
		OnStateChangeDelegate = Delegate;
	}

public:
	/// @brief Call this to pass in a delegate that will receive operation state change notifications
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAsyncOperationWidget")
	void SetOperationStateDelegate(const FOnSetOperationState& Delegate);

	/// @brief Call this to request that the underlying widget retry the async operation
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAsyncOperationWidget")
	void RequestOperationRetry();
};