/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Base/ModioCommonWidgetSwitcher.h"

#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"

void UModioCommonWidgetSwitcher::HandleSlateActiveIndexChanged(int32 ActiveIndex)
{
	Super::HandleSlateActiveIndexChanged(ActiveIndex);
	
	if (UModioCommonActivatableWidget* IncomingActivatable = Cast<UModioCommonActivatableWidget>(GetWidgetAtIndex(ActiveWidgetIndex)))
	{
		if (IncomingActivatable->GetAutoFocusOnActivation())
		{
			IncomingActivatable->FocusOnDesiredWidget();
		}
	}
}
