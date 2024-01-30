/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Foundation/Components/AsyncLoader/ModioCommonUIAsyncLoader.h"
#include "CommonActivatableWidget.h"

void UModioCommonUIAsyncLoader::NativeHandleAsyncOperationStateChange(EModioUIAsyncOperationWidgetState NewState)
{
	const bool bDifferentState = NewState != CurrentState;
	if (bDifferentState)
	{
		if (UCommonActivatableWidget* ContentWidget = Cast<UCommonActivatableWidget>(GetContentWidget()))
		{
			ContentWidget->DeactivateWidget();
		}
	}

	Super::NativeHandleAsyncOperationStateChange(NewState);

	if (bDifferentState)
	{
		if (UCommonActivatableWidget* ContentWidget = Cast<UCommonActivatableWidget>(GetContentWidget()))
		{
			ContentWidget->ActivateWidget();
		}
	}
}
