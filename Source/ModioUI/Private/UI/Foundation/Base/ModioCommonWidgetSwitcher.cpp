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

#include "ModioUI.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"

void UModioCommonWidgetSwitcher::HandleSlateActiveIndexChanged(int32 ActiveIndex)
{
	Super::HandleSlateActiveIndexChanged(ActiveIndex);

	// Automatically focus on the desired widget if it is UModioCommonActivatableWidget and has AutoFocusOnActivation set
	if (UModioCommonActivatableWidget* IncomingActivatable = Cast<UModioCommonActivatableWidget>(GetWidgetAtIndex(GetActiveWidgetIndex())))
	{
		if (IncomingActivatable->GetAutoFocusOnActivation())
		{
			IncomingActivatable->FocusOnDesiredWidget();
		}
	}

	OnActiveWidgetIndexChanged_BP.Broadcast(GetWidgetAtIndex(GetActiveWidgetIndex()), GetActiveWidgetIndex());
}
