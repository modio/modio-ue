/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/ModDetails/ModioAsyncOpWrapperWidget.h"

void UModioAsyncOpWrapperWidget::NativeRequestOperationRetry()
{
	if (ActualWidget)
	{
		IModioUIAsyncOperationWidget::Execute_RequestOperationRetry(ActualWidget.GetObject());
	}
	else
	{
		IModioUIAsyncOperationWidget::NativeRequestOperationRetry();
	}
}

void UModioAsyncOpWrapperWidget::SetActualAsyncOperationWidget(
	const TScriptInterface<IModioUIAsyncOperationWidget>& Widget)
{
	ActualWidget = Widget;
}

void UModioAsyncOpWrapperWidget::NativeNotifyOperationState(EModioUIAsyncOperationWidgetState NewState)
{
	if (ActualWidget)
	{
		IModioUIAsyncOperationWidget::Execute_NotifyOperationState(ActualWidget.GetObject(), NewState);
	}
	else
	{
		IModioUIAsyncOperationWidget::NativeNotifyOperationState(NewState);
	}
}
