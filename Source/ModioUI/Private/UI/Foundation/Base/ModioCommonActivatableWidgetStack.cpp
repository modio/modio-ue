/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Base/ModioCommonActivatableWidgetStack.h"
#include "Slate/SCommonAnimatedSwitcher.h"
#include "CommonActivatableWidget.h"

void UModioCommonActivatableWidgetStack::SynchronizeProperties()
{
	Super::SynchronizeProperties();

#if WITH_EDITOR
	if (IsDesignTime() && PreviewContentWidgetClass
		&& MySwitcher.IsValid())
	{
		if (PreviewContentWidget && MySwitcher->GetNumWidgets() > 0 && MySwitcher->GetWidget(0) == PreviewContentWidget->TakeWidget())
		{
			MySwitcher->GetChildSlot(0)->DetachWidget();
			PreviewContentWidget = nullptr;
		}
		PreviewContentWidget = CreateWidget<UCommonActivatableWidget>(this, PreviewContentWidgetClass);
		if (!MySwitcher->GetChildSlot(0))
		{
			MySwitcher->AddSlot()[SNullWidget::NullWidget];
		}
		if (PreviewContentWidget)
		{
			MySwitcher->GetChildSlot(0)->AttachWidget(PreviewContentWidget->TakeWidget());
			SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
#endif
}

UCommonActivatableWidget* UModioCommonActivatableWidgetStack::BP_AddWidgetSmart(TSubclassOf<UCommonActivatableWidget> ActivatableWidgetClass, bool bSkipIfAlreadyActive, bool bRemoveIfInStack)
{
	return AddWidgetSmart(ActivatableWidgetClass, bSkipIfAlreadyActive, bRemoveIfInStack);
}
