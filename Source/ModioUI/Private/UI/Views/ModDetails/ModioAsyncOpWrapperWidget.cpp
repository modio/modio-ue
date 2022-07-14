// Fill out your copyright notice in the Description page of Project Settings.

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
