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
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "UObject/Interface.h"

#include "IModioUIAsyncHandlerWidget.generated.h"

UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioUIAsyncHandlerWidget : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnChangeAsyncHandlerOperationState, EModioUIAsyncOperationWidgetState, NewState);

class MODIOUICORE_API IModioUIAsyncHandlerWidget : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeLinkAsyncOperationWidget(const TScriptInterface<IModioUIAsyncOperationWidget>& Widget) {}
	virtual EModioUIAsyncOperationWidgetState NativeGetAsyncOperationState() const { return EModioUIAsyncOperationWidgetState::Error; }
	virtual void NativeSetOnOperationStateDelegate(const FOnChangeAsyncHandlerOperationState& Delegate) {}

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAsyncHandlerWidgets")
	void LinkAsyncOperationWidget(const TScriptInterface<IModioUIAsyncOperationWidget>& Widget);
	void LinkAsyncOperationWidget_Implementation(const TScriptInterface<IModioUIAsyncOperationWidget>& Widget)
	{
		NativeLinkAsyncOperationWidget(Widget);
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAsyncHandlerWidgets")
	EModioUIAsyncOperationWidgetState GetAsyncOperationState() const;
	EModioUIAsyncOperationWidgetState GetAsyncOperationState_Implementation() const
	{
		return NativeGetAsyncOperationState();
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIAsyncHandlerWidgets")
	void SetOnOperationStateDelegate(const FOnChangeAsyncHandlerOperationState& Delegate);
	void SetOnOperationStateDelegate_Implementation(const FOnChangeAsyncHandlerOperationState& Delegate)
	{
		NativeSetOnOperationStateDelegate(Delegate);
	}
};