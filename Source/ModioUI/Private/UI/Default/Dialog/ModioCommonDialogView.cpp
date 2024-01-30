/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/Dialog/ModioCommonDialogView.h"

#include "Components/WidgetSwitcherSlot.h"
#include "ModioSubsystem.h"
#include "ModioUISubsystem.h"
#include "UI/Foundation/Base/Dialog/ModioCommonDialogMessageViewBase.h"
#include "UI/Foundation/Base/ModioCommonWidgetSwitcher.h"
#include "Core/ModioModInfoUI.h"
#include "ModioSettings.h"
#include "UI/Default/Dialog/ModioCommonDialogMessageView.h"
#include "UI/Default/Dialog/ModioCommonDialogInfo.h"

void UModioCommonDialogView::NativeOnActivated()
{
	if (DialogSwitcher)
	{
		DialogSwitcher->ClearChildren();
		TArray<UWidgetSwitcherSlot*> SwitcherSlots;

		if (DialogMessageViewClass)
		{
			DialogMessageView = CreateWidget<UModioCommonDialogMessageViewBase>(this, DialogMessageViewClass);
			if (DialogMessageView)
			{
				SwitcherSlots.Add(Cast<UWidgetSwitcherSlot>(DialogSwitcher->AddChild(DialogMessageView)));
				if (DataSource)
				{
					DialogMessageView->SetDataSource(DataSource);
				}
			}
		}

		for (UWidgetSwitcherSlot* SwitcherSlot : SwitcherSlots)
		{
			if (SwitcherSlot)
			{
				SwitcherSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				SwitcherSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
		}

		DialogSwitcher->SetActiveWidgetIndex(0);
	}

	Super::NativeOnActivated();
}

void UModioCommonDialogView::NativeOnDeactivated()
{
	if (DialogSwitcher)
	{
		DialogSwitcher->SetActiveWidgetIndex(0);
		DialogSwitcher->ClearChildren();
	}
	Super::NativeOnDeactivated();
}

void UModioCommonDialogView::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (DataSource) 
	{
		if (DialogMessageView)
		{
			DialogMessageView->SetDataSource(DataSource);
		}
	}
}

UWidget* UModioCommonDialogView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (DialogSwitcher && DialogSwitcher->GetChildrenCount() > 0)
	{
		if (UModioCommonActivatableWidget* ActivatableWidget = Cast<UModioCommonActivatableWidget>(DialogSwitcher->GetActiveWidget()))
		{
			return ActivatableWidget->GetDesiredFocusTarget();
		}
	}

	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonDialogView::OnDialogMessageViewSubmitClicked()
{
	DeactivateWidget();
}