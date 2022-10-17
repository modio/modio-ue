/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioModPropertyInspectorBase.h"
#include "Core/ModioModInfoUI.h"

void UModioModPropertyInspectorBase::SynchronizeProperties()
{
	UModioUserWidgetBase::SynchronizeProperties();
	if (UModioModInfoUI* UnderlyingData = Cast<UModioModInfoUI>(DataSource))
	{
		UpdatePropertyDisplay();
	}
}

void UModioModPropertyInspectorBase::NativeOnSetDataSource()
{
	UModioUserWidgetBase::NativeOnSetDataSource();
	if (UModioModInfoUI* UnderlyingData = Cast<UModioModInfoUI>(DataSource))
	{
		UpdatePropertyDisplay();
	}
}

void UModioModPropertyInspectorBase::UpdatePropertyDisplay()
{
	bRoutedUpdatePropertyDisplay = false;
	NativeOnUpdatePropertyDisplay();
	ensureMsgf(bRoutedUpdatePropertyDisplay, TEXT("Call Super::NativeOnUpdatePropertyDisplay in your overridden "
												  "implementation to ensure events are routed to blueprint"));
}

void UModioModPropertyInspectorBase::NativeOnUpdatePropertyDisplay()
{
	OnUpdatePropertyDisplay();
	bRoutedUpdatePropertyDisplay = true;
}
