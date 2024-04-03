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
#include "Components/PanelWidget.h"

void UModioCommonUIAsyncLoader::NativeHandleAsyncOperationStateChange(EModioUIAsyncOperationWidgetState NewState)
{
	const bool bDifferentState = NewState != CurrentState;
	if (bDifferentState)
	{
		if (UWidget* ContentWidget = GetContentWidget())
		{
			SetActivationState_Recursive(ContentWidget, false);
		}
	}

	Super::NativeHandleAsyncOperationStateChange(NewState);

	if (bDifferentState)
	{
		if (UWidget* ContentWidget = GetContentWidget())
		{
			SetActivationState_Recursive(ContentWidget, true);
		}
	}
}

void UModioCommonUIAsyncLoader::SetActivationState_Recursive(UWidget* Widget, bool bActivate)
{
	if (UCommonActivatableWidget* ActivatableWidget = Cast<UCommonActivatableWidget>(Widget))
	{
		bActivate ? ActivatableWidget->ActivateWidget() : ActivatableWidget->DeactivateWidget();
	}
	else if (UPanelWidget* PanelWidget = Cast<UPanelWidget>(Widget))
	{
		for (UWidget* ChildWidget : PanelWidget->GetAllChildren())
		{
			SetActivationState_Recursive(ChildWidget, bActivate);
		}
	}
}